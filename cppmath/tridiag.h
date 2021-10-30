#ifndef TRIDIAG_H
#define TRIDIAG_H

template <class T>
class Tridiag_Array2D
{
  private:
    T* d_;
	T* s_; // s[0] arbitrary
	T* e_; // e[n-1] arbitrary
    int n_;
    int *ref_count_;

	void initialize_(int n);
    void copy_(T* p, const T*  q, int len) const;
    void set_(const T& val);
    void destroy_();
	inline const T* begin_() const;
	inline T* begin_();

  public:

    typedef         T   value_type;

	         Tridiag_Array2D();
	explicit Tridiag_Array2D(int n);
	         Tridiag_Array2D(int n, T *d, T *s, T *e);
	         Tridiag_Array2D(int n, const T &a);
    inline   Tridiag_Array2D(const Tridiag_Array2D &A);
	inline   Tridiag_Array2D & operator=(const T &a);
	inline   Tridiag_Array2D & operator=(const Tridiag_Array2D &A);
	inline   Tridiag_Array2D & ref(const Tridiag_Array2D &A);
	         Tridiag_Array2D copy() const;
		     Tridiag_Array2D & inject(const Tridiag_Array2D & A);
	inline   T& d(int i);
	inline   T& e(int i);
	inline   T& s(int i);
	inline   const T& d(int i) const;
	inline   const T& e(int i) const;
	inline   const T& s(int i) const;
	inline 	 int dim1() const;
	inline   int dim() const;
	inline   int ref_count() const;

	inline const T* getd() const { return &(d_[0]); }
	inline const T* gete() const { return &(e_[0]); }
	inline const T* gets() const { return &(s_[0]); }

	inline T* getd() { return &(d_[0]); }
	inline T* gete() { return &(e_[0]); }
	inline T* gets() { return &(s_[0]); }

	~Tridiag_Array2D();
};

/**
	Null constructor.  Creates a 0-length (NULL) array.
	(Reference count is also zero.)
*/

template <class T>
Tridiag_Array2D<T>::Tridiag_Array2D() : d_(0), s_(0), e_(0), n_(0), ref_count_(0)
{
	ref_count_ = new int;
	*ref_count_ = 2;
}

/**
	Copy constructor. Array data is NOT copied, but shared.
	Thus, in Tridiag_Array2D B(A), subsequent changes to A will
	be reflected in B.  For an indepent copy of A, use
	Tridiag_Array2D B(A.copy()), or B = A.copy(), instead.
*/
template <class T>
Tridiag_Array2D<T>::Tridiag_Array2D(const Tridiag_Array2D<T> &A) : 
	d_(A.d_), s_(A.s_), e_(A.e_), 
	n_(A.n_), ref_count_(A.ref_count_)
{
	(*ref_count_) += 2;
}



/**
	Create a new array (vector) of length <b>n</b>,
	WIHOUT initializing array elements.
	To create an initialized array of constants, see Tridiag_Array2D(n,value).

	<p>
	This version avoids the O(n) initialization overhead and
	is used just before manual assignment.

	@param n the dimension (length) of the new matrix.
*/
template <class T>
Tridiag_Array2D<T>::Tridiag_Array2D(int n) : d_(0), s_(0), e_(0), n_(n), ref_count_(0)
{
	initialize_(n);
	ref_count_ = new int;
	*ref_count_ = 2;
}



/**
	Create a new array of length <b>n</b>,  initializing array elements to
	constant specified by argument.  Most often used to
	create an array of zeros, as in A(n, 0.0).

	@param n the dimension (length) of the new matrix.
	@param val the constant value to set all elements of the new array to.
*/
template <class T>
Tridiag_Array2D<T>::Tridiag_Array2D(int n, const T &val) : d_(0), s_(0), e_(0), n_(n) ,
	ref_count_(0)
{
	initialize_(n);
	set_(val);
	ref_count_ = new int;
	*ref_count_ = 2;

}

/**
	Create a new n-length array,  as a view of an existing one-dimensional
	C array.  (Note that the storage for this pre-existing array will
	never be destroyed by the Aray1DRef class.)

	@param n the dimension (length) of the new matrix.
	@param a the one dimensional C array to use as data storage for
		the array.
*/
template <class T>
Tridiag_Array2D<T>::Tridiag_Array2D(int n, T *d, T *s, T *e) : d_(d), s_(s), e_(e), n_(n) ,
	ref_count_(0)
{
	ref_count_ = new int;
	*ref_count_ = 3;		/* this avoid destorying original data. */

}

/**
	Reference
*/
template <class T>
inline T& Tridiag_Array2D<T>::d(int i)

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 0);
	assert(i < n_);
#endif
	return d_[i];
}

template <class T>
inline T& Tridiag_Array2D<T>::s(int i)

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 1);
	assert(i < n_);
#endif
	return s_[i];
}
template <class T>
inline T& Tridiag_Array2D<T>::e(int i)

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 0);
	assert(i < n_-1);
#endif
	return e_[i];
}

template <class T>
inline const T& Tridiag_Array2D<T>::d(int i) const

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 0);
	assert(i < n_);
#endif
	return d_[i];
}

template <class T>
inline const T& Tridiag_Array2D<T>::s(int i) const

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 1);
	assert(i < n_);
#endif
	return s_[i];
}
template <class T>
inline const T& Tridiag_Array2D<T>::e(int i) const

{
#ifdef TNT_BOUNDS_CHECK
	assert(i>= 0);
	assert(i < n_-1);
#endif
	return e_[i];
}

/**
	Assign all elemnts of A to a constant scalar.
*/
template <class T>
Tridiag_Array2D<T> & Tridiag_Array2D<T>::operator=(const T &a)
{
	set_(a);
	return *this;
}
/**
	Create a new of existing matrix.  Used in B = A.copy()
	or in the construction of B, e.g. Tridiag_Array2D B(A.copy()),
	to create a new array that does not share data.

*/
template <class T>
Tridiag_Array2D<T> Tridiag_Array2D<T>::copy() const
{
	Tridiag_Array2D A( n_);
	copy_(A.begin_(), begin_(), 3*n_);

	return A;
}


/**
	Copy the elements to from one array to another, in place.
	That is B.inject(A), both A and B must conform (i.e. have
	identical row and column dimensions).

	This differs from B = A.copy() in that references to B
	before this assignment are also affected.  That is, if
	we have
	<pre>
	Tridiag_Array2D A(n);
	Tridiag_Array2D C(n);
	Tridiag_Array2D B(C);        // elements of B and C are shared.

</pre>
	then B.inject(A) affects both and C, while B=A.copy() creates
	a new array B which shares no data with C or A.

	@param A the array from which elements will be copied
	@return an instance of the modifed array. That is, in B.inject(A),
	it returns B.  If A and B are not conformat, no modifications to
	B are made.

*/
template <class T>
Tridiag_Array2D<T> & Tridiag_Array2D<T>::inject(const Tridiag_Array2D &A)
{
	if (A.n_ == n_)
		copy_(begin_(), A.begin_(), 3*n_);

	return *this;
}





/**
	Create a reference (shallow assignment) to another existing array.
	In B.ref(A), B and A shared the same data and subsequent changes
	to the array elements of one will be reflected in the other.
	<p>
	This is what operator= calls, and B=A and B.ref(A) are equivalent
	operations.

	@return The new referenced array: in B.ref(A), it returns B.
*/
template <class T>
Tridiag_Array2D<T> & Tridiag_Array2D<T>::ref(const Tridiag_Array2D<T> &A)
{
	if (this != &A)
	{
		(*ref_count_) -= 2;
		if ( *ref_count_ < 2)
		{
			destroy_();
		}

		n_ = A.n_;
		d_ = A.d_;
		e_ = A.e_;
		s_ = A.s_;
		ref_count_ = A.ref_count_;

		(*ref_count_) += 2 ;

	}
	return *this;
}

/**
	B = A is shorthand notation for B.ref(A).
*/
template <class T>
Tridiag_Array2D<T> & Tridiag_Array2D<T>::operator=(const Tridiag_Array2D<T> &A)
{
	return ref(A);
}

/**
	@return the dimension (number of elements) of the array.
	This is equivalent to dim() and dim1().
*/
template <class T>
inline int Tridiag_Array2D<T>::dim1() const { return n_; }

/**
	@return the dimension (number of elements) of the array.
	This is equivalent to dim1() and dim1().
*/
template <class T>
inline int Tridiag_Array2D<T>::dim() const { return n_; }



/**
	@return the number of arrays that share the same storage area
	as this one.  (Must be at least one.)
*/
template <class T>
inline int Tridiag_Array2D<T>::ref_count() const
{
	return *ref_count_;
}

template <class T>
Tridiag_Array2D<T>::~Tridiag_Array2D()
{
	(*ref_count_) -= 2;

	if (*ref_count_ < 2)
		destroy_();
}

/* private internal functions */

template <class T>
void Tridiag_Array2D<T>::initialize_(int n)
{
	d_ = new T[3*n];
	s_ = d_ + n;
	e_ = s_ + n;
	n_ = n;
}

template <class T>
void Tridiag_Array2D<T>::set_(const T& a)
{
	T *begin = &(v_[0]);
	T *end = begin + 3*n_;

	for (T* p=begin; p<end; p++)
		*p = a;

}

template <class T>
void Tridiag_Array2D<T>::copy_(T* p, const T* q, int len) const
{
	T *end = p + len;
	while (p<end )
		*p++ = *q++;

}

template <class T>
void Tridiag_Array2D<T>::destroy_()
{

	if (d_ != 0)
	{
		if (*ref_count_<1) delete[] (d_);

	}

	if (ref_count_ != 0)
		delete ref_count_;
}

/**
	@returns location of first element, i.e. A[0] (mutable).
*/
template <class T>
const T* Tridiag_Array2D<T>::begin_() const { return &(d_[0]); }

/**
	@returns location of first element, i.e. A[0] (mutable).
*/
template <class T>
T* Tridiag_Array2D<T>::begin_() { return &(d_[0]); }


template <class T>
ostream& operator<<(ostream &s, const Tridiag_Array2D<T> &A)
{
    int N=A.dim1()-1;

    s << N+1 << "\n";
    s << "xxxx\t" << A.d(0) << "\t" << A.e(0) << "\n";
    for (int j=1; j<N; j++)
    {
       s << A.s(j) << "\t" << A.d(j) << "\t" << A.e(j) << "\n";
    }
    s << A.s(j) << "\t" << A.d(j) << "\t" << "xxxx\n";
    s << "\n";

    return s;
}



#endif