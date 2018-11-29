
#include <z/core.h>
#include <z/system.h>
#include <z/file.h>

#include <z/compiler/preprocTags.h>
// #include <dlfcn.h>

typedef z::compiler::preprocTags* (* pptfunc)();

int main()
{
	z::system::console console;
	z::file::library lang;

	z::compiler::preprocTags* tags = NULL;

	lang.load("../zinglang/zasm.so");

	if (lang.bad())
	{
		zstring("Load failed!").writeln(console);
	}
	else
	{
		auto raw = (pptfunc)lang.symbol("preprocTags");
		if (raw)
		{
			tags = raw();
		}
		else
		{
			zstring("Symbol DNE!").writeln(console);
		}
	}

	if (!tags) tags = new z::compiler::preprocTags;

	auto directive = tags->directive();
	auto directend = tags->directend();

	if (directive.length())
	{
		(zstring("Directive = (")+directive+")").writeln(console);
		if (directend.length())
			(zstring("Directend = (")+directend+")").writeln(console);
		else
			zstring("No directive end pattern specified!").writeln(console);
	}
	else
		zstring("No directive start pattern defined.").writeln(console);

	lang.unload();

	return 0;
}
