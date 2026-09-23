#!/usr/bin/env python3
"""Run an Emscripten test page in headless Chromium and return its exit status."""

import argparse
from functools import partial
from http.server import SimpleHTTPRequestHandler, ThreadingHTTPServer
import os
from pathlib import Path
import sys
from threading import Thread
from urllib.parse import quote, urlencode

from playwright.sync_api import sync_playwright


class Handler(SimpleHTTPRequestHandler):
    def log_message(self, format, *args):
        pass


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("page", type=Path)
    parser.add_argument("--artifacts", type=Path, required=True)
    parser.add_argument("--timeout", type=int, default=90, help="Timeout in seconds")
    parser.add_argument("--browser", default=os.environ.get("MYGUI_TEST_BROWSER"))
    arguments = sys.argv[1:]
    separator = arguments.index("--") if "--" in arguments else len(arguments)
    args = parser.parse_args(arguments[:separator])
    test_args = arguments[separator + 1:]
    page_file = args.page.resolve(strict=True)
    args.artifacts.mkdir(parents=True, exist_ok=True)
    messages = []
    errors = []
    status = 1
    server = ThreadingHTTPServer(("127.0.0.1", 0), partial(Handler, directory=str(page_file.parent)))
    thread = Thread(target=server.serve_forever, daemon=True)
    thread.start()
    try:
        with sync_playwright() as playwright:
            with playwright.chromium.launch(
                executable_path=args.browser,
                args=["--use-angle=swiftshader", "--enable-unsafe-swiftshader"],
            ) as browser:
                page = browser.new_page(viewport={"width": 256, "height": 256}, device_scale_factor=1)
                page.on("console", lambda message: messages.append(message.text))
                page.on("pageerror", lambda error: errors.append(str(error)))
                try:
                    query = urlencode([("arg", value) for value in test_args])
                    url = f"http://127.0.0.1:{server.server_port}/{quote(page_file.name)}?{query}"
                    page.goto(url, wait_until="commit", timeout=args.timeout * 1000)
                    page.wait_for_function("window.testResult !== undefined", timeout=args.timeout * 1000)
                    result = page.evaluate("window.testResult")
                    if result.get("error"):
                        errors.append(result["error"])
                    status = 0 if result["status"] == 0 and not errors else 1
                except Exception as error:
                    errors.append(str(error))
                if status:
                    try:
                        page.screenshot(path=str(args.artifacts / "browser.png"), timeout=5000)
                        captures = page.evaluate("""() => {
                            const fs = window.Module && Module.FS;
                            if (!fs || !fs.analyzePath('/artifacts').exists) return {};
                            const files = {};
                            for (const name of fs.readdir('/artifacts')) {
                                if (name.endsWith('.ppm'))
                                    files[name] = Array.from(fs.readFile('/artifacts/' + name));
                            }
                            return files;
                        }""")
                        for name, data in captures.items():
                            (args.artifacts / Path(name).name).write_bytes(bytes(data))
                    except Exception as error:
                        errors.append(f"Cannot save browser artifacts: {error}")
    except Exception as error:
        errors.append(str(error))
    finally:
        server.shutdown()
        server.server_close()
        thread.join()
        output = "\n".join(messages + [f"BROWSER ERROR: {error}" for error in errors])
        (args.artifacts / "browser.log").write_text(output + "\n")
        print(output)
    return status


if __name__ == "__main__":
    sys.exit(main())
