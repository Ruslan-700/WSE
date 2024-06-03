#include "presentation.h"

#include "wb.h"

using namespace wb;

int presentation_manager::addPresentation(const presentation &newPresentation)
{
	presentation *newPresentations = rgl::_new<presentation>(this->num_presentations + 1);

	for (int i = 0; i < this->num_presentations; ++i)
	{
		newPresentations[i] = this->presentations[i];
	}
	rgl::_free(this->presentations);
	this->presentations = newPresentations;
	this->presentations[this->num_presentations] = newPresentation;

	return this->num_presentations++;
}

bool presentation_manager::removePresentation(int index)
{
	if (index < 0)
		index += this->num_presentations;

	if (index < 0 || index >= this->num_presentations)
		return false;


	size_t newTriggersSize = (this->num_presentations - 1) * sizeof(presentation);
	presentation *newPresentations = (presentation*)malloc(newTriggersSize);

	int oldIndex = 0;
	int newIndex = 0;
	while (oldIndex < this->num_presentations)
	{
		if (oldIndex != index)
		{
			void *oldIndexPtr = &(this->presentations[oldIndex]);
			void *newIndexPtr = &(newPresentations[newIndex]);
			memcpy_s(newIndexPtr, sizeof(presentation), oldIndexPtr, sizeof(presentation));

			newIndex++;
		}

		oldIndex++;
	}

	free(this->presentations);
	this->presentations = newPresentations;
	this->num_presentations--;

	return true;
}