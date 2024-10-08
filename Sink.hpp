#ifndef _DISPLAY_
#define _DISPLAY_
#include "Text.hpp"

const size_t INITCOLUMNSCOUNT = 30;
const size_t INITCOLWIDTH = 50;
class Display
{
public:
	Display() = default;
	virtual ~Display() = default;
	virtual Display* clone() const;
	virtual void displayFile(const Text& content) const;
};

class WrapDisplay : public Display
{
public:
	WrapDisplay(const MyVector<MyString>& operands);
	virtual WrapDisplay* clone() const override;
	virtual void displayFile(const Text& content) const override;
private:
	size_t columnsCount;
};


class CenterDisplay : public Display
{
public:
	CenterDisplay(const MyVector<MyString>& operands);
	virtual CenterDisplay* clone() const override;
	virtual void displayFile(const Text& content) const override;
private:
	size_t width = 0;
};

#endif