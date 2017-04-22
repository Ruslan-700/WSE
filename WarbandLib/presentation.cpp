#include "presentation.h"

#include "wb.h"

using namespace wb;

int presentation_manager::addPresentation(const presentation &newPresentation)
{
	size_t oldPresentationsSize = this->num_presentations * sizeof(presentation);
	size_t newPresentationsSize = oldPresentationsSize + sizeof(presentation);

	void *oldPresentations = (void*)this->presentations;
	void *newPresentations = malloc(newPresentationsSize);

	memcpy_s(newPresentations, newPresentationsSize, oldPresentations, oldPresentationsSize);
	free(oldPresentations);
	this->presentations = (presentation*)newPresentations;

	this->presentations[this->num_presentations].id.initialize();
	this->presentations[this->num_presentations].simple_triggers.simple_triggers = rgl::_new<simple_trigger>(0);
	//this->presentations[this->num_presentations].simple_triggers.num_simple_triggers = 1;
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