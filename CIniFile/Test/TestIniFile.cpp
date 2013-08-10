// TestCIniFile::cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;
#include "IniFile.h"

void Show(string FileName)
{
	cout << endl 
		 << "++++++++++++++++++++++++++++++++++++++++"
		 << endl
		 << "+ Contents of the file are below       +"
		 << endl
		 << "++++++++++++++++++++++++++++++++++++++++"
		 << endl
		 << CIniFile::Content(FileName)
		 << endl
		 << "++++++++++++++++++++++++++++++++++++++++"
		 << endl << endl;
	system ("PAUSE"); 
	system("cls");
}

int _tmain(int argc, _TCHAR* argv[])
{
	//CIniFile IniFile;
	string FileName = "test.ini";

	// Create a new file
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to create a new file called \"test.ini\"" << endl << endl;
	cout << "string FileName = \"test.ini\";" << endl;
	cout << "CIniFile::Create(FileName);" << endl << endl;
	if (CIniFile::Create(FileName)) cout << "File was successfully created" << endl << endl;
	else cout << "Failed to create the file" << endl << endl;
	Show(FileName);

	// Create a new section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to create a new section called [MySection]" << endl << endl;
	cout << "CIniFile::AddSection(\"MySection\", FileName);" << endl << endl;
	if (CIniFile::AddSection("MySection",FileName)) cout << "Section was successfully created" << endl << endl;
	else cout << "Failed to create the section" << endl << endl;
	Show(FileName);

	// Add a key to the section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add a new key/value (MyKey=MyValue) to [MySection]" << endl << endl;
	cout << "CIniFile::SetValue(\"MyKey\",\"MyValue\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::SetValue("MyKey","MyValue","MySection",FileName)) cout << "Record was successfully created" << endl << endl;
	else cout << "Failed to create the record" << endl << endl;
	Show(FileName);

	// Add a key and create a section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add a new key/value (TestKey=TestValue)" << endl << "and create a new section [TestSection] at the same time" << endl << endl;
	cout << "CIniFile::SetValue(\"TestKey\",\"TestValue\",\"TestSection\",FileName);" << endl << endl;
	if (CIniFile::SetValue("TestKey","TestValue","TestSection",FileName)) cout << "Record and section were successfully created" << endl << endl;
	else cout << "Failed to create the record and section" << endl << endl;
	Show(FileName);

	// Change a key value
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to change the key/value for (MyKey=MyValue) to (MyKey=YourValue)" << endl << endl;
	cout << "CIniFile::SetValue(\"MyKey\",\"YourValue\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::SetValue("MyKey","YourValue","MySection",FileName)) cout << "Record was successfully changed" << endl << endl;
	else cout << "Failed to change the record" << endl << endl;
	Show(FileName);

	// Get a value
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to get the value of MyKey" << endl << endl;
	cout << "CIniFile::GetValue(\"MyKey\",\"MySection\",FileName);" << endl << endl;
	string v = CIniFile::GetValue("MyKey","MySection",FileName);
	cout << "The value of MyKey is: " << v.c_str() << endl << endl;
	Show(FileName);

	// Get a list of Sections
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to get a list of sections" << endl << endl;
	cout << "CIniFile::GetSectionNames(FileName);" << endl << endl;
	vector<string> s = CIniFile::GetSectionNames(FileName);
	cout << "The sections are returned as a std::vector<std::string>\n\n";
	for(int i=0; i < (int)s.size(); i++)
		cout << s[i].c_str() << endl;
	Show(FileName);

	// Section Exists
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to verify that [MySection] exists" << endl << endl;
	cout << "CIniFile::SectionExists(\"MySection\",FileName);" << endl << endl;
	if (CIniFile::SectionExists("MySection",FileName)) cout << "Section exists" << endl << endl;
	else cout << "Section does not exist" << endl << endl;
	Show(FileName);

	// Record Exists
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to verify that MyKey exists" << endl << endl;
	cout << "CIniFile::RecordExists(\"MyKey\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::RecordExists("MyKey","MySection",FileName)) cout << "Record exists" << endl << endl;
	else cout << "Record does not exist" << endl << endl;
	Show(FileName);

	// Case Sensitive
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "BE CAREFUL - functions in CIniFile are CASE-SENSITIVE" << endl << endl;
	cout << "CIniFile::RecordExists(\"mykey\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::RecordExists("mykey","MySection",FileName)) cout << "Record exists" << endl << endl;
	else cout << "Record does not exist" << endl << endl;
	Show(FileName);
    
	// Add a comment to the section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add comments to [MySection]" << endl << endl;
	cout << "CIniFile::SetSectionComments(\"# This Section was created by CIniFile\\n\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::SetSectionComments("# This Section was created by CIniFile\n","MySection",FileName)) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(FileName);

	// Add a comment to the record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to add comments to MyKey" << endl << endl;
	cout << "CIniFile::SetRecordComments(\"# This Key was created by CIniFile\\n\",\"MyKey\",\"MySection\",FileName);" << endl << endl;
	if (CIniFile::SetRecordComments("# This Key was created by CIniFile\n","MyKey","MySection",FileName)) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(FileName);

	// Rename Section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to rename [MySection] to [YourSection]" << endl << endl;
	cout << "CIniFile::RenameSection(\"MySection\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::RenameSection("MySection","YourSection",FileName)) cout << "Section was successfully changed" << endl << endl;
	else cout << "Failed to change the section" << endl << endl;
	Show(FileName);

	// Multiple comments
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Multiple comments can be added by putting \\n# in the comments string" << endl << endl;
	cout << "CIniFile::SetSectionComments(\"# This Section was created by CIniFile\\n# Kids, don't try this at home \\n\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::SetSectionComments("# This Section was created by CIniFile\n# Kids, don't try this at home","YourSection",FileName)) cout << "Comments were successfully added" << endl << endl;
	else cout << "Failed to add the comments" << endl << endl;
	Show(FileName);

	// Remove comments
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Comments are removed by setting them to \"\"" << endl << endl;
	cout << "CIniFile::SetRecordComments(\"\",\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::SetRecordComments("","MyKey","YourSection",FileName)) cout << "Comments were successfully removed" << endl << endl;
	else cout << "Failed to remove the comments" << endl << endl;
	Show(FileName);

	// Comment entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to comment the entire section [YourSection]" << endl << endl;
	cout << "CIniFile::CommentSection(\"#\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::CommentSection('#',"YourSection",FileName)) cout << "Section was successfully commented" << endl << endl;
	else cout << "Failed to comment the section" << endl << endl;
	Show(FileName);

	// UnComment entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to un-comment the entire section [YourSection]" << endl << endl;
	cout << "CIniFile::UnCommentSection(\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::UnCommentSection("YourSection",FileName)) cout << "Section was successfully un-commented" << endl << endl;
	else cout << "Failed to un-comment the section" << endl << endl;
	Show(FileName);

	// Comment a single record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to comment the record MyKey" << endl << endl;
	cout << "(Note that both # and ; are recognized as commented lines by CIniFile)" << endl << endl;
	cout << "CIniFile::CommentRecord(CIniFile::CommentChar::Pound,\"MyKey\",\"YourSection\",FileName);" << endl << endl;
 	if (CIniFile::CommentRecord(CIniFile::CommentChar::Pound,"MyKey","YourSection",FileName)) cout << "Record was successfully commented" << endl << endl;
	else cout << "Failed to comment the record" << endl << endl;
	Show(FileName);

	// UnComment a single record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to un-comment the record MyKey" << endl << endl;
	cout << "CIniFile::UnCommentRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::UnCommentRecord("MyKey","YourSection",FileName)) cout << "Record was successfully un-commented" << endl << endl;
	else cout << "Failed to un-comment the record" << endl << endl;
	Show(FileName);

	// Sort
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to sort the file - false means ASCENDING, true means DESCENDING" << endl << endl;
	cout << "(Note that the comments will stay with their targets)" << endl << endl;
	cout << "CIniFile::Sort(FileName,false);" << endl << endl;
	if (CIniFile::Sort(FileName,false)) cout << "File was successfully sorted" << endl << endl;
	else cout << "Failed to sort the file" << endl << endl;
	Show(FileName);

	// Delete entire section
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to delete the entire section [TestSection]" << endl << endl;
	cout << "CIniFile::DeleteSection(\"TestSection\",FileName);" << endl << endl;
	if (CIniFile::DeleteSection("TestSection",FileName)) cout << "Section was successfully deleted" << endl << endl;
	else cout << "Failed to delete the section" << endl << endl;
	Show(FileName);

	// Delete record
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Attempting to delete the record <yKey" << endl << endl;
	cout << "CIniFile::DeleteRecord(\"MyKey\",\"YourSection\",FileName);" << endl << endl;
	if (CIniFile::DeleteRecord("MyKey","YourSection",FileName)) cout << "Record was successfully deleted" << endl << endl;
	else cout << "Failed to delete the record" << endl << endl;
	Show(FileName);

	// Content
	cout << "TestIniFile - Demo program for the CIniFile Class" << endl << endl;
	cout << "Finally, the content of the file can be retrieved as a std::string" << endl << endl;
	cout << "CIniFile::Content(FileName);" << endl << endl;
	cout << "The contents of the file throughout this demo have used this function to display the contents below" << endl;
	Show(FileName);

	return 0;
}