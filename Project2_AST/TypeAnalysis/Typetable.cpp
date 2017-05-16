#include "TypeTable.h"

using namespace Utilities;
using Utils = StringHelper;

//template<typename TableRecord>
//void TypeTable::showRecord(TableRecord& record)
//{
//	std::cout << "\n  ";
//	std::cout << std::setw(16) << record.fileName();
//	std::cout << std::setw(8) << record.name();
//	std::cout << std::setw(12) << record.type();
//	for (auto ns : record)
//		std::cout << std::setw(16) << ns;
//}
//
//template<typename TableRecord>
//void TypeTable::showRecordHeader()
//{
//	std::cout << std::left << "\n  ";
//	std::cout << std::setw(16) << "Filename";
//	std::cout << std::setw(8) << "Name";
//	std::cout << std::setw(12) << "Type";
//	std::cout << std::setw(16) << "Namespaces" << "\n";
//	std::cout << " " << std::setw(16) << std::string(14, '-');
//	std::cout << std::setw(8) << std::string(6, '-');
//	std::cout << std::setw(12) << std::string(10, '-');
//	std::cout << std::setw(16) << std::string(14, '-');
//}

#ifdef TEST_TYPETBL

void main()
{
	Utils::Title("MT4Q1 - TypeTable");
	putline();

	TypeTable<TypeTableRecord> table;

	TypeTableRecord record;
	record.name() = "X";
	record.type() = "class";
	record.fileName() = "X.h";
	record.addNameSpace("TypeAnalysis");
	record.addNameSpace("MT4");

	table.addRecord(record);

	record.name() = "fun";
	record.type() = "method";
	record.fileName() = "X.h";
	record.addNameSpace("TypeAnalysis");
	record.addNameSpace("MT4");

	table.addRecord(record);

	showTypeTable(table);
	std::cout << "\n\n";
}

#endif // TEST_TYPETBL