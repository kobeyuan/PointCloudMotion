#ifndef _EXAMPLE_SET_H
#define _EXAMPLE_SET_H
#include "sample.h"
#include <vector>
/* 
	A wrapper class of a set of examples
*/



class SampleSet
{
public:

	static SampleSet& get_instance()
	{
		static SampleSet	instance;
		return instance;
	}


	void push_back( Sample*  );
	bool empty(){ return set_.empty(); }
	void clear();

	Sample* operator[](size_t idx)
	{
		if (idx >= set_.size())
		{
			return nullptr;
		}
		return set_[idx];
	}

	size_t size(){ return set_.size(); }

private:
	SampleSet(){}
	~SampleSet(){}

	SampleSet(const SampleSet& );
	void operator=(const SampleSet&);

private:
	std::vector<Sample*>	set_;

};

#endif