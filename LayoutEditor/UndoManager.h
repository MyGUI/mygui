#include <vector>

/// Fixed size cyclic buffer.
template<class T>
class CyclicBuffer
{
public:
	typedef std::vector<T> container_type;                      ///< Underlying container type.
	typedef typename std::vector<T>::value_type value_type;     ///< Element type.
	typedef typename std::vector<T>::size_type size_type;       ///< Size and offset type.

	/// Fixed size c-tor.
	explicit CyclicBuffer( size_type size ) : c( new value_type[size] ), pos( -1 ), count( 0 ), size( size ) {}

	/// D-tor.
	~CyclicBuffer() { delete [] c; }

	/// Is buffer empty?
	bool IsEmpty() const { return GetSize() == 0; }
	/// Is buffer full?
	bool IsFull() const { return GetSize() == GetCapacity(); }
	/// Get size.
	size_type GetSize() const { return count; }
	/// Get capacity.
	size_type GetCapacity() const { return size; }

	/// Peek n-th element for read (0 = newest element).
	const value_type& operator[]( size_type offset ) const { return c[( size + pos - offset ) % size]; }
	/// Peek n-th element for edit (0 = newest element).
	value_type& operator[]( size_type offset ) { return c[( size + pos - offset ) % size]; }
	/// Peek newest element for read.
	const value_type& Front() const { return (*this)[0]; }
	/// Peek newest element for edit.
	value_type& Front() { return (*this)[0]; }
	/// Peek oldest element for read.
	const value_type& Back() const { return (*this)[count - 1]; }
	/// Peek oldest element for edit.
	value_type& Back() { return (*this)[count - 1]; }

	/// Clear buffer and all allocated elements.
	void Clear() { while ( !IsEmpty() ) Pop(); }
	/// Clear buffer, no elements d-tors will be called.
	void ClearFast() { count = 0; }

	/// Expand buffer, no element init will be done (uses Pop()).
	void Expand() { if ( IsFull() ) Pop(); pos = ( pos + 1 ) % size; ++count; }
	/// Expand buffer, no element init will be done (uses PopFast()).
	void ExpandFast() { if ( IsFull() ) PopFast(); pos = ( pos + 1 ) % size; ++count; }

	/// Push element into buffer tail (uses Expand())..
	void Push( const value_type &v ) { Expand(); c[pos] = v; }
	/// Push element into buffer tail (uses Expand()).
	void Push( value_type &v ) { Expand(); c[pos] = v; }
	/// Push element into buffer tail (uses ExpandFast()).
	void PushFast( const value_type &v ) { ExpandFast(); c[pos] = v; }
	/// Push element into buffer tail (uses ExpandFast()).
	void PushFast( value_type &v ) { ExpandFast(); c[pos] = v; }

	/// Pop last element from buffer.
	void Pop()
	{
		if ( !IsEmpty() )
		{
			Back() = value_type();
			PopFast();
		}
	}

  /// Fast pop last element from buffer, no element d-top will be called.
	void PopFast()
	{
		if ( !IsEmpty() )
			--count;
	}

private:
	size_type pos, count, size;
	value_type *c;
};

struct WidgetContainer;
class EditorWidgets;

enum {OP_NONE, OP_CREATE, OP_DELETE, OP_CHANGE};
struct UndoOperation
{
	UndoOperation():operation(OP_NONE){};
	UndoOperation(int _operation, WidgetContainer* _widget_container):
		operation(_operation)
	{
		containers.push_back(_widget_container);
	}
	UndoOperation(int _operation, std::vector<WidgetContainer*> _containers):
		operation(_operation)
	{
		containers = _containers;
	}

	int operation;
	std::vector<WidgetContainer*> containers;
};

enum {PR_DEFAULT, PR_POSITION};
class UndoManager
{
public:
	UndoManager(EditorWidgets * ew);

	void undo();
	void redo();
	void addValue( const UndoOperation & _undoOp, int _property = PR_DEFAULT);
	int getCount();

	UndoOperation getLast( );
	UndoOperation getAt( size_t _pos );

	size_t pos;

private:
	CyclicBuffer<UndoOperation> operations;
	int last_property;

	EditorWidgets * ew;
};
