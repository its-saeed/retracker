#include "OnDestructionRunner.h"

OnDestructionRunner::OnDestructionRunner(const OnDestructionRunner::Function& func)
: fn(func)
{

}

OnDestructionRunner::~OnDestructionRunner()
{
	fn();
}
