/*
 *		nana::pgitems_creator Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PGITEMS_CREATOR_H
#define NANA_CREATOR_PGITEMS_CREATOR_H

#include "nana_extra/pgitems.h"
#include "ctrls/property.h"


using pgitem = nana::drawerbase::propertygrid::pgitem;



/// class pg_filename
class pg_filename
	: public nana::pg_string_button
{
public:
	pg_filename() = default;

	pg_filename(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

	virtual void value(const std::string& value) override;

	void add_filter(const std::string& description, const std::string& filetype);

protected:
	virtual void create(nana::window wd) override;

	std::vector<std::pair<std::string, std::string>> filters_;
};



/// class pg_image
class pg_image
	: public nana::pg_string_button
{
public:
	pg_image() = default;

	pg_image(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

	virtual void value(const std::string& value) override;

protected:
	virtual void create(nana::window wd) override;
};



/// class pg_folder
class pg_folder
	: public nana::pg_string_button
{
public:
	pg_folder() = default;

	pg_folder(const std::string& label, const std::string& value)
		: pg_string_button(label, value)
	{}

	virtual void value(const std::string& value) override;

protected:
	virtual void create(nana::window wd) override;
};



/// class pg_collection
class pg_collection //TODO
	: public nana::pg_string_button
{
public:
	pg_collection() = default;

	pg_collection(const std::string& label, ctrls::pg_type type, std::vector<ctrls::properties_collection>* items)
		: pg_string_button(label, ""), type_(type), items_(items)
	{}

	virtual void value(const std::string& value) override {}
	virtual void items(std::vector<ctrls::properties_collection>* items) { items_ = items; }

protected:
	virtual void create(nana::window wd) override;

	ctrls::pg_type type_;
	std::vector<ctrls::properties_collection>* items_{ 0 };
};


/// class pg_layout_weight
class pg_layout_weight
	: public nana::pg_string
{
public:
	pg_layout_weight() = default;

	pg_layout_weight(const std::string& label, const std::string& value)
		: pg_string(label, value)
	{}

	virtual void value(const std::string& value) override;

protected:
	virtual void create(nana::window wd) override;

	virtual void draw_value(nana::paint::graphics* graph, nana::rectangle rect, const int txtoff, nana::color bgcolor, nana::color fgcolor) const override;

	mutable ::nana::combox	cmb_;
};


#endif //NANA_CREATOR_PGITEMS_CREATOR_H
