#ifndef ONDESTRUCTIONRUNNER_H
#define ONDESTRUCTIONRUNNER_H

#include <functional>

class OnDestructionRunner
{
public:
	typedef std::function<void(void)> Function;
	OnDestructionRunner(const Function& func);

	~OnDestructionRunner();

private:
	Function fn;
};

#endif // ONDESTRUCTIONRUNNER_H
