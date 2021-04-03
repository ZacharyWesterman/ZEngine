#include "grammar/preprocessor.hpp"
#include <z/all.hpp>

int main()
{
	z::system::stdout out;
	grammar::preprocess("examples/data/maingrammar.gr", out);

	return 0;
}
