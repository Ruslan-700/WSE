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

	presentation *newPresentations = rgl::_new<presentation>(this->num_presentations - 1);
	for (int i = 0; i < index; ++i)
	{
		newPresentations[i] = this->presentations[i];
	}
	for (int i = index; i < this->num_presentations - 1; ++i)
	{
		newPresentations[i] = this->presentations[i + 1];
	}
	rgl::_free(this->presentations);
	this->presentations = newPresentations;
	this->num_presentations--;

	return true;
}