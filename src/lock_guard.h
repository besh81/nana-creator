/*
*      lock_guard Implementation
*
*      Nana C++ Library - Creator
*      Author: besh81
*/

#ifndef NANA_CREATOR_LOCK_GUARD_H
#define NANA_CREATOR_LOCK_GUARD_H

class lock_guard
{
public:
	lock_guard(bool* flag, bool lock_as_true = true)
	{
		flag_ = flag;
		*flag_ = lock_as_true;
	}

	~lock_guard()
	{
		*flag_ = !(*flag_);
	}

private:
	bool* flag_;

};

#endif //NANA_CREATOR_LOCK_GUARD_H
