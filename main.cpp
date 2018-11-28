
#include <z/core.h>
#include <z/system.h>
#include <z/file.h>

#include <z/compiler/preprocTags.h>
// #include <dlfcn.h>

#define s(x) z::core::string<>(x)

typedef z::compiler::preprocTags* (* pptfunc)();

int main()
{
	z::system::console console;
	z::file::library lang;

	z::compiler::preprocTags* tags = NULL;

	lang.load("../zinglang/zasm.so");

	if (lang.bad())
	{
		s("Load failed!").writeln(console);
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
			s("Symbol DNE!").writeln(console);
		}
	}

	if (!tags) tags = new z::compiler::preprocTags;

	auto directive = tags->directive();
	auto directend = tags->directend();

	if (directive.length())
	{
		(s("Directive = (")+directive+")").writeln(console);
		if (directend.length())
			(s("Directend = (")+directend+")").writeln(console);
		else
			s("No directive end pattern specified!").writeln(console);
	}
	else
		s("No directive start pattern defined.").writeln(console);

	lang.unload();

	return 0;
}
