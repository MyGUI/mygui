#ifndef SIGSLOT_H__
#define SIGSLOT_H__

#include <set>
#include <list>

namespace sigslot
{

	class has_slots;

	namespace impl
	{
		template<class arg1_type>
		class connection_base1
		{
		public:
			typedef connection_base1<arg1_type> connection_base_type;
			virtual has_slots* getdest() const = 0;
			virtual void emit(arg1_type) = 0;
			virtual connection_base_type* clone() = 0;
			virtual connection_base_type* duplicate(has_slots* pnewdest) = 0;
		};

		class signal_base
		{
		public:
			virtual void slot_disconnect(has_slots* pslot) = 0;
			virtual void slot_duplicate(const has_slots* poldslot, has_slots* pnewslot) = 0;
		};
	}

	class has_slots
	{
	private:
		typedef std::set<impl::signal_base *> sender_set;
		typedef sender_set::const_iterator const_iterator;

	public:
		has_slots()
		{
		}

		has_slots(const has_slots& hs)
		{
			const_iterator it = hs.m_senders.begin();
			const_iterator itEnd = hs.m_senders.end();

			while (it != itEnd)
			{
				(*it)->slot_duplicate(&hs, this);
				m_senders.insert(*it);
				++it;
			}
		}

		void signal_connect(impl::signal_base* sender)
		{
			m_senders.insert(sender);
		}

		void signal_disconnect(impl::signal_base* sender)
		{
			m_senders.erase(sender);
		}

		virtual ~has_slots()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			const_iterator it = m_senders.begin();
			const_iterator itEnd = m_senders.end();

			while (it != itEnd)
			{
				(*it)->slot_disconnect(this);
				++it;
			}

			m_senders.erase(m_senders.begin(), m_senders.end());
		}

	private:
		sender_set m_senders;
	};

	namespace impl
	{
		template<class arg1_type>
		class signal_base1 :
			public signal_base
		{
		public:
			typedef std::list<connection_base1<arg1_type> *>  connections_list;
			typedef signal_base1<arg1_type> signal_base_type;

			signal_base1()
			{
			}

			signal_base1(const signal_base_type& s) :
				signal_base(s)
			{
				connections_list::const_iterator it = s.m_connected_slots.begin();
				connections_list::const_iterator itEnd = s.m_connected_slots.end();

				while (it != itEnd)
				{
					(*it)->getdest()->signal_connect(this);
					m_connected_slots.push_back((*it)->clone());

					++it;
				}
			}

			void slot_duplicate(const has_slots* oldtarget, has_slots* newtarget)
			{
				connections_list::iterator it = m_connected_slots.begin();
				connections_list::iterator itEnd = m_connected_slots.end();

				while (it != itEnd)
				{
					if ((*it)->getdest() == oldtarget)
					{
						m_connected_slots.push_back((*it)->duplicate(newtarget));
					}

					++it;
				}
			}

			~signal_base1()
			{
				disconnect_all();
			}

			void disconnect_all()
			{
				connections_list::const_iterator it = m_connected_slots.begin();
				connections_list::const_iterator itEnd = m_connected_slots.end();

				while (it != itEnd)
				{
					(*it)->getdest()->signal_disconnect(this);
					delete *it;

					++it;
				}

				m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
			}

			void disconnect(has_slots* pclass)
			{
				connections_list::iterator it = m_connected_slots.begin();
				connections_list::iterator itEnd = m_connected_slots.end();

				while (it != itEnd)
				{
					if ((*it)->getdest() == pclass)
					{
						delete *it;
						m_connected_slots.erase(it);
						pclass->signal_disconnect(this);
						return;
					}

					++it;
				}
			}

			void slot_disconnect(has_slots* pslot)
			{
				connections_list::iterator it = m_connected_slots.begin();
				connections_list::iterator itEnd = m_connected_slots.end();

				while (it != itEnd)
				{
					connections_list::iterator itNext = it;
					++itNext;

					if ((*it)->getdest() == pslot)
					{
						m_connected_slots.erase(it);
					}

					it = itNext;
				}
			}

		protected:
			connections_list m_connected_slots;
		};

		template<class dest_type, class arg1_type>
		class connection1 :
			public connection_base1<arg1_type>
		{
		public:
			connection1()
			{
				pobject = 0;
				pmemfun = 0;
			}

			connection1(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type))
			{
				m_pobject = pobject;
				m_pmemfun = pmemfun;
			}

			virtual connection_base_type* clone()
			{
				return new connection1<dest_type, arg1_type>(*this);
			}

			virtual connection_base_type* duplicate(has_slots* pnewdest)
			{
				return new connection1<dest_type, arg1_type>((dest_type *)pnewdest, m_pmemfun);
			}

			virtual void emit(arg1_type a1)
			{
				(m_pobject->*m_pmemfun)(a1);
			}

			virtual has_slots* getdest() const
			{
				return m_pobject;
			}

		private:
			dest_type* m_pobject;
			void (dest_type::* m_pmemfun)(arg1_type);
		};

	}

	template<class arg1_type>
	class signal1 :
		public impl::signal_base1<arg1_type>
	{
	public:
		signal1()
		{
		}

		signal1(const signal1<arg1_type>& s) :
			signal_base_type(s)
		{
		}

		template<class desttype>
		void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
		{
			impl::connection1<desttype, arg1_type>* conn = new impl::connection1<desttype, arg1_type>(pclass, pmemfun);
			m_connected_slots.push_back(conn);
			pclass->signal_connect(this);
		}

		void emit(arg1_type a1)
		{
			connections_list::const_iterator itNext, it = m_connected_slots.begin();
			connections_list::const_iterator itEnd = m_connected_slots.end();

			while (it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1);

				it = itNext;
			}
		}

		void operator()(arg1_type a1)
		{
			emit(a1);
		}
	};

} // namespace sigslot

#endif // SIGSLOT_H__
