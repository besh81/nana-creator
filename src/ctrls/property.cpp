/*
 *		ctrls::property Implementation
 *
 *      part of Nana Creator (https://github.com/besh81/nana-creator)
 *      Author: besh81
 */

#include <assert.h>
#include "ctrls/property.h"


namespace ctrls
{
	namespace impl
	{
		inline bool get_as_bool(const std::string& value)
		{
			// only look at first char
			char first = value[0];
			// 1*, t* (true), T* (True), y* (yes), Y* (YES)
			return (first == '1' || first == 't' || first == 'T' || first == 'y' || first == 'Y');
		}
	}//end namespace impl


	property_proxy& property_proxy::name(const std::string& name)
	{
		_prop->name = name;
		return *this;
	}
	std::string property_proxy::name() const
	{
		return _prop->name;
	}

	property_proxy& property_proxy::value(const std::string& value)
	{
		_prop->value = value;
		return *this;
	}
	std::string property_proxy::value() const
	{
		return _prop->value;
	}

	std::string property_proxy::defvalue() const
	{
		return _prop->defvalue;
	}

	property_proxy& property_proxy::label(const std::string& label)
	{
		_prop->label = label;
		return *this;
	}
	std::string property_proxy::label() const
	{
		return _prop->label;
	}

	property_proxy& property_proxy::category(const std::string& category)
	{
		_prop->category = category;
		return *this;
	}
	std::string property_proxy::category() const
	{
		return _prop->category;
	}

	property_proxy& property_proxy::type(const pg_type type)
	{
		_prop->type = type;
		return *this;
	}
	pg_type property_proxy::type() const
	{
		return _prop->type;
	}

	property_proxy& property_proxy::enabled(const std::string& property, bool value)
	{
		_prop->enabled_prop = property;
		_prop->enabled_value = value;
		return *this;
	}
	std::string property_proxy::enabled() const
	{
		return _prop->enabled_prop;
	}
	bool property_proxy::enabled_value() const
	{
		return _prop->enabled_value;
	}



	bool property_proxy::operator==(const property_proxy& r) const
	{
		return (_prop == r._prop);
	}
	bool property_proxy::operator!=(const property_proxy& r) const
	{
		return (_prop != r._prop);
	}



	const std::string& property_proxy::as_string(const std::string& def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? _prop->value : def);
	}
	int property_proxy::as_int(int def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? std::stoi(_prop->value) : def);
	}
	unsigned int property_proxy::as_uint(unsigned int def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? std::stoul(_prop->value) : def);
	}
	double property_proxy::as_double(double def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? std::stod(_prop->value) : def);
	}
	float property_proxy::as_float(float def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? std::stof(_prop->value) : def);
	}
	bool property_proxy::as_bool(bool def) const
	{
		return !_prop ? def : (!_prop->value.empty() ? impl::get_as_bool(_prop->value) : def);
	}



	property_proxy& property_proxy::value(const char* value)
	{
		if(_prop)
			_prop->value = value;
		return *this;
	}
	property_proxy& property_proxy::value(int value)
	{
		if(_prop)
			_prop->value = std::to_string(value);
		return *this;
	}
	property_proxy& property_proxy::value(unsigned int value)
	{
		if(_prop)
			_prop->value = std::to_string(value);
		return *this;
	}
	property_proxy& property_proxy::value(double value)
	{
		if(_prop)
			_prop->value = std::to_string(value);
		return *this;
	}
	property_proxy& property_proxy::value(float value)
	{
		if(_prop)
			_prop->value = std::to_string(value);
		return *this;
	}
	property_proxy& property_proxy::value(bool value)
	{
		if(_prop)
			_prop->value = value ? "true" : "false";
		return *this;
	}


	property_proxy& property_proxy::operator=(const std::string& value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(const char* value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(int value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(unsigned int value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(double value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(float value_)
	{
		value(value_);
		return def_as_value();
	}
	property_proxy& property_proxy::operator=(bool value_)
	{
		value(value_);
		return def_as_value();
	}



	property_proxy& property_proxy::def_as_value()
	{
		if(_prop)
			_prop->defvalue = _prop->value;
		return *this;
	}



	properties_collection::~properties_collection()
	{
		_props.clear();
	}


	bool properties_collection::empty() const
	{
		return _props.empty();
	}

	property_proxy properties_collection::append(const std::string& name)
	{
		property_t prop;
		prop.name = name;
		_props.push_back(prop);
		return property_proxy{ &_props.back() };
	}
	property_proxy properties_collection::append(const property_t& prop)
	{
		_props.push_back(prop);
		return property_proxy{ &_props.back() };
	}

	void properties_collection::remove(const std::string& name)
	{
		for (auto i = _props.begin(); i < _props.end(); ++i)
			if(i->name == name)
			{
				_props.erase(i);
				return;
			}
	}

	void properties_collection::clear()
	{
		_props.clear();
	}

	property_proxy properties_collection::property(const std::string& name)
	{
		for(auto i = _props.begin(); i < _props.end(); ++i)
			if(i->name == name)
				return property_proxy{ &*i };

		return property_proxy{};
	}
	
	size_t properties_collection::count() const
	{
		return _props.size();
	}

	property_proxy properties_collection::operator[](size_t index)
	{
		assert(index < _props.size());
		return property_proxy{ &_props[index] };
	}

}//end namespace ctrls
