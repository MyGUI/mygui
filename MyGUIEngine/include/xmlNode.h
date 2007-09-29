#ifndef _XMLNODE_H_
#define _XMLNODE_H_

#include <vector>
#include <string>
#include "xmlDefine.h"

namespace xml
{

	class xmlNode;
	class xmlDocument;

	typedef xmlNode * xmlNodePtr;
	typedef std::pair<std::string, std::string> PairAttributes;
	typedef std::vector<PairAttributes> VectorAttributes;
	typedef std::vector<xmlNodePtr> VectorNode;

	class xmlNode
	{
		friend  xmlDocument;

	private:
		~xmlNode()
		{
			for (VectorNode::iterator iter=m_childs.begin(); iter!=m_childs.end(); iter++) {
				delete *iter;
			}
			m_childs.clear();
		}

		xmlNode(const std::string &_name, xmlNodePtr _parent, bool _info = false, const std::string & _body = "") : 
			m_name(_name),
			m_body(_body),
			m_parent(_parent),
			m_info(_info)
		{
			
		}

		void save(std::ofstream & _stream, size_t _level)
		{
			// сначала табуляции намутим
			for (size_t tab=0; tab<_level; tab++) _stream  << "    ";

			// теперь заголовок тега
			if (m_info) _stream << "<?";
			else _stream << "<";
			_stream << m_name;

			for (VectorAttributes::iterator iter = m_attributes.begin(); iter != m_attributes.end(); iter ++) {
				_stream << " " << iter->first << "=\"" << iter->second << "\"";
			}

			bool empty = m_childs.empty();
			// если детей нет то закрываем
			if (empty && m_body.empty()) {
				if (m_info) _stream << "?>\n";
				else _stream << "/>\n";
			} else {
				_stream << ">";
				if (!empty) _stream << "\n";
				// если есть тело то сначало оно
				if (!m_body.empty()) {
					if (!empty) {
						for (size_t tab=0; tab<=_level; tab++) _stream  << "    ";
						_stream << m_body << "\n";
					} else _stream << m_body;
				}
				// если есть детишки путь сохранятся
				for (size_t child=0; child<m_childs.size(); child++) {
					m_childs[child]->save(_stream, _level + 1);
				}

				if (!empty) {for (size_t tab=0; tab<_level; tab++) _stream  << "    ";}
				_stream << "</" << m_name << ">\n";
			}

		}

	public:
		xmlNodePtr createChild(const std::string & _name, const std::string & _body = "") 
		{
			xmlNodePtr node = new xmlNode(_name, this, false, _body);
			m_childs.push_back(node);
			return node;
		}

		inline void addAttributes(const std::string &_key, const std::string &_value)
		{
			m_attributes.push_back(PairAttributes(_key, _value));
		}

		inline void addBody(const std::string &_body)
		{
			if (!m_body.empty()) m_body += " ";
			m_body += _body;
		}

		inline const std::string & getName() {return m_name;};
		inline const std::string & getBody() {return m_body;};
		inline const VectorAttributes & getAttributes() {return m_attributes;};
		inline VectorNode & getChilds() {return m_childs;};
		inline xmlNodePtr getParent() {return m_parent;};

	private:
		std::string m_name;
		std::string m_body;
		VectorAttributes m_attributes;
		VectorNode m_childs;
		xmlNodePtr m_parent;
		bool m_info;
	};

} // namespace xml

#endif
