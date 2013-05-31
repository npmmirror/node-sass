#include "stdlib.h"
#include "string.h"
#include "sass_values.h"

union Sass_Value make_sass_boolean(int val)
{
	union Sass_Value v;
	v.boolean.tag = SASS_BOOLEAN;
	v.boolean.value = val;
	return v;
}

union Sass_Value make_sass_number(double val)
{
	union Sass_Value v;
	v.number.tag = SASS_NUMBER;
	v.number.value = val;
	return v;
}

union Sass_Value make_sass_percentage(double val)
{
	union Sass_Value v;
	v.percentage.tag = SASS_PERCENTAGE;
	v.percentage.value = val;
	return v;
}

union Sass_Value make_sass_dimension(double val, const char* unit)
{
	union Sass_Value v;
	v.dimension.tag = SASS_DIMENSION;
	v.dimension.value = val;
	char* copy = (char*) malloc(sizeof(char)*(strlen(unit)+1));
	strcpy(copy, unit);
	v.dimension.unit = copy;
	return v;
}

union Sass_Value make_sass_color(double r, double g, double b, double a)
{
	union Sass_Value v;
	v.color.tag = SASS_COLOR;
	v.color.r = r;
	v.color.g = g;
	v.color.b = b;
	v.color.a = a;
	return v;
}

union Sass_Value make_sass_string(const char* val)
{
	union Sass_Value v;
	v.string.tag = SASS_STRING;
	char* copy = (char*) malloc(sizeof(char)*(strlen(val)+1));
	strcpy(copy, val);
	v.string.value = copy;
	return v;
}

union Sass_Value make_sass_list(size_t len, enum Sass_Separator sep)
{
	union Sass_Value v;
	v.list.tag = SASS_LIST;
	v.list.separator = sep;
	v.list.values = (union Sass_Value*) malloc(sizeof(union Sass_Value)*len);
	return v;
}

union Sass_Value make_sass_error(const char* msg)
{
	union Sass_Value v;
	v.error.tag = SASS_ERROR;
	char* copy = (char*) malloc(sizeof(char)*(strlen(msg)+1));
	strcpy(copy, msg);
	v.error.message = copy;
	return v;
}


void free_sass_value(union Sass_Value v)
{
	switch (v.unknown.tag)
	{
		case SASS_STRING: {
		  free(v.string.value);
		  v.string.value = NULL;
		} break;
		case SASS_LIST: {
			size_t i;
			for (i = 0; i < v.list.length; ++i) free_sass_value(v.list.values[i]);
			free(v.list.values);
			v.list.values = NULL;
			v.list.length = 0;
		} break;
		case SASS_ERROR: {
			free(v.error.message);
			v.error.message = NULL;
		} break;
	}
}