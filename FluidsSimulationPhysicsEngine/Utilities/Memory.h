#pragma once
class MemoryUtilities
{
public:

	template <class T> static void delete_var(T*& variableToDelete) {
		if (variableToDelete != NULL) {
			delete variableToDelete;
		}
		variableToDelete = NULL;
	}

	MemoryUtilities();
	~MemoryUtilities();
};

