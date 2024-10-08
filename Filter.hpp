#ifndef _FILTER_
#define _FILTER_
#include "Text.hpp"
#include "helpers.hpp"
#include <cstring>
#include <string>

enum class EmptyPlacesPlacement
{
	Ivalid = -1,
	Beginning,
	Middle,
	End,
	Count
};

//BASEFILTER
//===================================================================
class BaseFilter
{
public:
	virtual ~BaseFilter() = default;
	virtual BaseFilter* clone() const = 0;
	virtual void filterText(Text& text) const = 0;
	virtual void specificFilterLogic(MyString& line) const {};
};
//===================================================================


//INRANGEFILTER
//===================================================================
class InRangeFilter : public BaseFilter
{
public:
	InRangeFilter(const MyVector<MyString>&, size_t pos = 0);
	virtual ~InRangeFilter() = default;
	virtual void filterText(Text& text) const override;
protected:
	int index1;
	int index2;
};
//===================================================================


//LINEFILTER
//===================================================================
class LineFilter : public BaseFilter
{
public:
	LineFilter(const MyVector<MyString>& operands);
	virtual ~LineFilter() = default;
protected:
	size_t lineIndex = 0;
};
//===================================================================


//REPLACEWORDFILTER
//===================================================================
class ReplaceWordFilter : public InRangeFilter
{
public:
	ReplaceWordFilter(const MyVector<MyString>&, size_t pos = 2);
	virtual BaseFilter* clone() const override;
	virtual ~ReplaceWordFilter() = default;
	virtual void specificFilterLogic(MyString& line) const override;
protected:
	MyString wordToReplaceWith;
	MyString wordToBeReplaced;
	bool replaceWithEmptyString = false;
};
//===================================================================


//RMOVALWORDFILTER
//===================================================================
class RemovalWordFilter : public ReplaceWordFilter
{
public:
	RemovalWordFilter(const MyVector<MyString>&);
	virtual RemovalWordFilter* clone() const override;
};
//===================================================================

//REMOVEEMPTYPLACESFILTER
//===================================================================
class RemoveEmptyPlacesFilter : public InRangeFilter
{
public:
	RemoveEmptyPlacesFilter(const MyVector<MyString>&);
	virtual ~RemoveEmptyPlacesFilter() = default;
	virtual RemoveEmptyPlacesFilter* clone() const override = 0;
};
//===================================================================

//REMOVEEMPTYPLACESATFRONTFILTER
//===================================================================
class RemoveEmptyPlacesAtFront : public RemoveEmptyPlacesFilter
{
public:
	RemoveEmptyPlacesAtFront(const MyVector<MyString>&);
	virtual void specificFilterLogic(MyString& line) const override;
	virtual RemoveEmptyPlacesAtFront* clone() const override;
};
//===================================================================


//REMOVEEMPTYPLACESATBEGINNINGFILTER
//===================================================================
class RemoveEmptyPlacesAtEnd : public RemoveEmptyPlacesFilter
{
public:
	RemoveEmptyPlacesAtEnd(const MyVector<MyString>&);
	virtual void specificFilterLogic(MyString& line) const override;
	virtual RemoveEmptyPlacesAtEnd* clone() const override;
};
//===================================================================


//ENCODEFILTER
//===================================================================
class EncodeFilter : public InRangeFilter
{
public:
	EncodeFilter(const MyVector<MyString>&, int (*setFunEncode) (int));
	virtual ~EncodeFilter() = default;
	virtual void specificFilterLogic(MyString& line) const override;
	virtual EncodeFilter* clone() const override;

private:
	bool isQuote(const MyString& line) const;
private:
	int (*funEncode) (int);
};
//===================================================================


//LEXSORTFILTER
//===================================================================
class LexSortFilter : public InRangeFilter
{
public:
	LexSortFilter(const MyVector<MyString>&, bool (*comparator)(const MyString&, const MyString&));
	virtual void filterText(Text& text) const override;
	virtual LexSortFilter* clone() const override;

private:
	bool (*comparator)(const MyString&, const MyString&);
	bool sortableLine(const MyString& line) const;
};
//===================================================================

//SHUFFLEFILTER
//===================================================================
class ShuffleFilter : public InRangeFilter
{
public:
	ShuffleFilter(const MyVector<MyString>&);
	virtual void filterText(Text& text) const override;
	virtual ShuffleFilter* clone() const override;

};
//===================================================================

//REMOVEDUPLICATESFILTER
//===================================================================
class RemoveDublicatesFilter : public InRangeFilter
{
public:
	RemoveDublicatesFilter(const MyVector<MyString>&);
	virtual void filterText(Text& text) const override;
	virtual RemoveDublicatesFilter* clone() const override;
};
//===================================================================

//REMOVELINEFILTER
//===================================================================
class RemoveLineFilter : public LineFilter
{
public:
	RemoveLineFilter(const MyVector<MyString>&);
	virtual void filterText(Text& text) const override;
	virtual RemoveLineFilter* clone() const override;
};
//===================================================================

//ADDDLINEFILTER
//===================================================================
class AddLineFilter : public LineFilter
{
public:
	AddLineFilter(const MyVector<MyString>&);
	virtual void filterText(Text& text) const override;
	virtual AddLineFilter* clone() const override;

private:
	MyString line;
};
//===================================================================

//REPLACELINEFILTER
//===================================================================
class ReplaceLineFilter : public LineFilter
{
public:
	ReplaceLineFilter(const MyVector<MyString>&);
	virtual void filterText(Text& text) const override;
	virtual ReplaceLineFilter* clone() const override;

private:
	MyString line;
};
//===================================================================
#endif