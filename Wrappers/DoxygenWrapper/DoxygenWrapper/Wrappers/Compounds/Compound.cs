using System;
using System.Collections.Generic;
using System.Xml;
using System.IO;
using System.Collections;

namespace DoxygenWrapper.Wrappers.Compounds
{
	public class Compound :
		IEnumerable<Compound>
	{
		public string RefID
		{
			get { return mRefID; }
		}

		public string Kind
		{
			get { return mKind; }
		}

		public string Name
		{
			get { return mName; }
		}

		public bool Member
		{
			get { return mMember; }
		}

		public Compound Parent
		{
			get { return mParent; }
		}

		public void Initialise(XmlNode _node)
		{
			mInitialiseNode = _node;
			mRefID = _node.Attributes["refid"].Value;
			mKind = _node.Attributes["kind"].Value;
			mName = _node["name"].InnerText;
			mMember = _node.Name == "member";
		}

		public void Parse(string _folder)
		{
			//ConsoleUtility.WriteLine("Parse {0}", RefID);

			XmlDocument doc = new XmlDocument();
			doc.Load(Path.Combine(_folder, string.Format("{0}.xml", mRefID)));

			XmlNode node = doc.DocumentElement["compounddef"];
			if (node.Attributes["id"].Value != mRefID ||
				node.Attributes["kind"].Value != mKind ||
				node["compoundname"].InnerText != mName)
			{
				ConsoleUtility.WriteErrorLine("Error file format or type : {0} != {1} || {2} != {3} || {4} != {5}",
					node.Attributes["id"].Value, mRefID,
					node.Attributes["kind"].Value, mKind,
					node["compoundname"].InnerText, mName);
			}

			Parse(node);
		}

		public void Parse(XmlNode _node)
		{
			mParseNode = _node;

			OnParse(_node);
		}

		public void AddChild(Compound _child)
		{
			if (_child.mParent != null)
			{
				// рутовый парент
				if (_child.mParent.RefID == "")
				{
					_child.mParent.RemoveChild(_child);
				}
				else
				{
					if (_child.mParent == this)
					{
						ConsoleUtility.WriteErrorLine("Child {0} already added for this {1}", _child.RefID, _child.mParent.RefID);
					}
					else
					{
						ConsoleUtility.WriteErrorLine("Child {0} already have parent {1}", _child.RefID, _child.mParent.RefID);
					}
					return;
				}
			}

			_child.mParent = this;
			mChilds.Add(_child);
		}

		private void RemoveChild(Compound _child)
		{
			if (_child.mParent == this)
			{
				_child.mParent = null;
				mChilds.Remove(_child);
			}
			else
			{
				ConsoleUtility.WriteErrorLine("Child {0} have not this parent {1}", _child.RefID, mRefID);
			}
		}

		protected virtual void OnParse(XmlNode _node)
		{
		}

		public IEnumerator<Compound> GetEnumerator()
		{
			return mChilds.GetEnumerator();
		}

		IEnumerator IEnumerable.GetEnumerator()
		{
			return mChilds.GetEnumerator();
		}

		private string mRefID = "";
		private string mKind = "";
		private string mName = "";
		private bool mMember;
		private List<Compound> mChilds = new List<Compound>();
		private Compound mParent;
		private XmlNode mInitialiseNode;
		private XmlNode mParseNode;
	}
}
