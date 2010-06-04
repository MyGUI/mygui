using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Mogre;
using System.Threading;

namespace MogreFramework
{
    public partial class OgreWindow : Form
    {
        #region Fields
        Root mRoot = null;
        RenderWindow mWindow = null;
        Camera mCamera = null;
        Viewport mViewport = null;
        SceneManager mSceneMgr = null;
        #endregion

        #region Properties
        public Root Root
        {
            get
            {
                return mRoot;
            }
        }

        public RenderWindow RenderWindow
        {
            get
            {
                return mWindow;
            }

            protected set
            {
                mWindow = value;
            }
        }

        public Camera Camera
        {
            get
            {
                return mCamera;
            }

            protected set
            {
                mCamera = value;
            }
        }

        public Viewport Viewport
        {
            get
            {
                return mViewport;
            }

            protected set
            {
                mViewport = value;
            }
        }

        public SceneManager SceneManager
        {
            get
            {
                return mSceneMgr;
            }

            protected set
            {
                mSceneMgr = value;
            }
        }
        #endregion

        #region Events
        public delegate void SceneEventHandler(OgreWindow win);
        public event SceneEventHandler SceneCreating;
        #endregion

        #region Constructor
        public OgreWindow()
        {
            InitializeComponent();
            this.Icon = global::MogreFramework.Properties.Resources.OgreHead;
        }
        #endregion

        #region Public Methods
        /// <summary>
        /// Begins the rendering loop.  This automatically calls InitializeOgre if it has not
        /// been done already.
        /// </summary>
        public void Go()
        {
            if (mRoot == null)
                InitializeOgre();

            Show();

            while (mRoot != null && mRoot.RenderOneFrame())
                System.Windows.Forms.Application.DoEvents();
        }

        /// <summary>
        /// Initializes ogre and shows the splash screen as it loads.
        /// </summary>
        public void InitializeOgre()
        {
            if (mRoot != null)
                throw new Exception("Ogre is already initialized!");

            Splash splash = new Splash();
            splash.Show();

            try
            {
                splash.Increment("Creating the root object...");
                mRoot = new Root();

                splash.Increment("Loading resources...");
                InitResources();

                splash.Increment("Setting up DirectX...");
                SetupDirectX();

                splash.Increment("Creating the window...");
                CreateRenderWindow(this.Handle);

                splash.Increment("Initializing resources...");
                InitializeResources();

                splash.Increment("Creating Ogre objects...");
                CreateSceneManager();
                CreateCamera();
                CreateViewport();

                splash.Increment("Creating input handler...");
                CreateInputHandler();

                splash.Increment("Creating scene...");
                Disposed += new EventHandler(OgreWindow_Disposed);
                OnSceneCreating();
            }
            finally
            {
                splash.Close();
                splash.Dispose();
            }
        }
        #endregion

        #region Protected Virtual Methods
        /// <summary>
        /// Creates the scene manager which will be used.
        /// If you override this function, you must set the SceneManager property of this
        /// class to be the result.  Example:
        ///   this.SceneManager = this.Root.CreateSceneManager(...);
        /// </summary>
        protected virtual void CreateSceneManager()
        {
            mSceneMgr = mRoot.CreateSceneManager(SceneType.ST_GENERIC, "Main SceneManager");
        }

        /// <summary>
        /// Creates the camera for this class.
        /// If you override this function, you must set the Camera property of this class
        /// to be the result.  Example:
        ///   this.Camera = this.SceneManager.CreateCamera(...);
        /// </summary>
        protected virtual void CreateCamera()
        {
            mCamera = mSceneMgr.CreateCamera("MainCamera");
            mCamera.NearClipDistance = 1;
            mCamera.Position = new Vector3(0, 0, 300);
            mCamera.LookAt(Vector3.ZERO);
        }

        /// <summary>
        /// Creates the viewport for this class.
        /// If you override this function, you must set the viewport property of this class
        /// to be the result:
        ///   this.Viewport = this.RenderWindow.AddViewport(...);
        /// </summary>
        protected virtual void CreateViewport()
        {
            mViewport = mWindow.AddViewport(mCamera);
            mViewport.BackgroundColour = new ColourValue(0.0f, 0.0f, 0.0f, 1.0f);
        }

        /// <summary>
        /// Creates the input handler for this class.  If you wish to create your own input handler,
        /// override this function and set it up.
        /// </summary>
        protected virtual void CreateInputHandler()
        {
            new DefaultInputHandler(this);
        }

        /// <summary>
        /// Creates the render window for this class.
        /// If you override this function, you must set the RenderWindow property.  Example:
        ///   this.RenderWindow = this.Root.CreateRenderWindow(...);
        /// </summary>
        /// <param name="handle">The window handle to render ogre in.</param>
        protected virtual void CreateRenderWindow(IntPtr handle)
        {
            mRoot.Initialise(false, "Main Ogre Window");
            if (handle != IntPtr.Zero)
            {
                NameValuePairList misc = new NameValuePairList();
                misc["externalWindowHandle"] = handle.ToString();
                mWindow = mRoot.CreateRenderWindow("Autumn main RenderWindow", 800, 600, false, misc);
            }
            else
            {
                mWindow = mRoot.CreateRenderWindow("Autumn main RenderWindow", 800, 600, false);
            }
        }

        /// <summary>
        /// Initializes the resources which the program uses.
        /// </summary>
        protected virtual void InitResources()
        {
            ConfigFile cf = new ConfigFile();
            cf.Load("resources.cfg", "\t:=", true);
            ConfigFile.SectionIterator seci = cf.GetSectionIterator();
            String secName, typeName, archName;

            while (seci.MoveNext())
            {
                secName = seci.CurrentKey;
                ConfigFile.SettingsMultiMap settings = seci.Current;
                foreach (KeyValuePair<string, string> pair in settings)
                {
                    typeName = pair.Key;
                    archName = pair.Value;
                    ResourceGroupManager.Singleton.AddResourceLocation(archName, typeName, secName);
                }
            }
        }

        #region Event Triggers
        protected virtual void OnSceneCreating()
        {
            if (SceneCreating != null)
                SceneCreating(this);
        }
        #endregion

        #endregion

        #region Private Methods
        void SetupDirectX()
        {
            RenderSystem rs = mRoot.GetRenderSystemByName("Direct3D9 Rendering Subsystem");
            mRoot.RenderSystem = rs;
            rs.SetConfigOption("Full Screen", "No");
            rs.SetConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
        }

        void OgreWindow_Disposed(object sender, EventArgs e)
        {
            mRoot.Dispose();
            mRoot = null;
            mWindow = null;
            mCamera = null;
            mViewport = null;
            mSceneMgr = null;
        }

        static void InitializeResources()
        {
            TextureManager.Singleton.DefaultNumMipmaps = 5;
            ResourceGroupManager rm = ResourceGroupManager.Singleton;

            rm.InitialiseAllResourceGroups();
        }
        #endregion
    }
}
