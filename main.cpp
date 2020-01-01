#include <z/core.h>
#include <z/file.h>
#include <z/system/console.h>

void err(zpath msg, int lineNumber)
{
	z::system::console console;

	msg = zpath("ERR (") + lineNumber + "): " + msg;
	msg.writeln(console);
}

void parseDataVal(zstring& line, int lineNumber, int& arrayDepth)
{
	auto parts = z::core::split(line,zstring(":"));
	parts[0].trimIn();

	auto trimVal = parts[1].trim();
	auto valType = trimVal.type();

	if ((parts[0] == "string") || (parts[0] == "s"))
	{
		line = zstring("<string>") + parts[1];
	}
	else if ((parts[0] == "float") || (parts[0] == "f"))
	{
		line = zstring("<float>") + trimVal;
		if (!(valType & z::core::zstr::floating))
		{
			err(zpath("\"") + trimVal + "\" is not a float!", lineNumber);
		}
	}
	else if ((parts[0] == "int") || (parts[0] == "i"))
	{
		line = zstring("<int>") + trimVal;
		if (!(valType & z::core::zstr::integer))
		{
			err(zpath("\"") + trimVal + "\" is not an integer!", lineNumber);
		}
	}
	else if ((parts[0] == "complex") || (parts[0] == "c"))
	{
		line = zstring("<complex>") + trimVal;
		if (!(valType & z::core::zstr::complex))
		{
			err(zpath("\"") + trimVal + "\" is not a complex value!", lineNumber);
		}
	}
	else if ((parts[0] == "array") || (parts[0] == "a"))
	{
		line = zstring()
		if (!(valType & z::core::zstr::integer))
		{
			err(zpath("\"") + trimVal + "\" is not a valid array length!", lineNumber);
		}
	}
	else
	{
		err(zpath("Unknown data type \"") + parts[0] + "\"", lineNumber);
	}
}


int main()
{
	z::system::console console;
	z::file::inputStream file ("examples/data/program1.txt");

	z::core::array<zstring> lines;

	bool inData = false;

	zstring line;
	int lineNumber = 0;
	while(!file.empty())
	{
		line.readln(file);

		if (line.beginsWith(".txt")) //begin program block (this is default)
		{
			inData = false;
			line = "BEGIN TXT BLOCK";
		}
		else if (line.beginsWith(".data")) //begin data block
		{
			inData = true;
			line = "BEGIN DATA BLOCK";
		}
		else if (line[0] != ';')//skip comments
		{
			if (inData) //in data section
			{
				if (line.find(':') >= 0)
				{
					parseDataVal(line, lineNumber);
				}
				else
				{
					err("Missing data type", lineNumber);
				}
				line.insert("D: ",0);
			}
			else //in program section
			{
				line.insert("T: ",0);
			}
		}

		lines.add(line);
		++lineNumber;
	}

	zpath().writeln(console);
	for (auto& line : lines) line.writeln(console);

	return 0;
}
