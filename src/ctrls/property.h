/*
 *		ctrls::property Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_PROPERTY_H
#define NANA_CREATOR_PROPERTY_H

#include <string>
#include <vector>
#include <nana/any.hpp>


namespace ctrls
{
	enum class pg_type
	{
		string,
		string_int,
		string_uint,
		choice,
		check,
		spin,
		color,
		color_inherited,
		collection
	};

	struct property_t
	{
		std::string		name;
		std::string		value;

		//
		std::string		label;
		std::string		category;
		pg_type			type;
		nana::any		type_hints;
	};



	class property_proxy
	{
	public:
		// Default constructor
		property_proxy() = default;
		explicit property_proxy(property_t* prop)
			: _prop(prop)
		{}


		property_proxy& name(const std::string& name);
		std::string name() const;

		property_proxy& value(const std::string& value);
		std::string value() const;

		property_proxy& label(const std::string& label);
		std::string label() const;
		
		property_proxy& category(const std::string& category);
		std::string  category() const;

		property_proxy& type(const pg_type type);
		pg_type type() const;

		property_proxy& type_hints(const nana::any type);
		nana::any type_hints() const;


		// Comparison operators (compares wrapped property pointers)
		bool operator==(const property_proxy& r) const;
		bool operator!=(const property_proxy& r) const;


		// Get property value as a number, or the default value if conversion did not succeed or property is empty
		const std::string& as_string(const std::string& def = "") const;
		int as_int(int def = 0) const;
		unsigned int as_uint(unsigned int def = 0) const;
		double as_double(double def = 0.f) const;
		float as_float(float def = 0.f) const;
		bool as_bool(bool def = false) const;


		// Set property value with type conversion (numbers are converted to strings, boolean is converted to "true"/"false")
		property_proxy& value(const char* value);
		property_proxy& value(int value);
		property_proxy& value(unsigned int value);
		property_proxy& value(double value);
		property_proxy& value(float value);
		property_proxy& value(bool value);


		// Set property value (equivalent to set_value without error checking)
		property_proxy& operator=(const std::string& value);
		property_proxy& operator=(const char* value);
		property_proxy& operator=(int value);
		property_proxy& operator=(unsigned int value);
		property_proxy& operator=(double value);
		property_proxy& operator=(float value);
		property_proxy& operator=(bool value);


		// Undocumentated methods for internal use
		property_t* _m_prop() const;

	private:
		property_t*		_prop{ 0 };
	};



	class properties_collection
	{
	public:
		properties_collection() = default;
		~properties_collection();


		// Check if node is empty.
		bool empty() const;


		// Add property with specified name. Returns added property, or empty property on errors.
		property_proxy append(const std::string& name);
		property_proxy append(const property_t& prop);

		// Remove property with specified name
		void remove(const std::string& name);

		// Remove all the properties
		void clear();

		// Get property with the specified name
		property_proxy property(const std::string& name);


		// Get number of properties
		size_t count() const;

		// Get property at the specified index
		property_proxy operator[](size_t index);


	protected:
		std::vector<property_t>		_props;
	};


}//end namespace ctrls

#endif //NANA_CREATOR_PROPERTY_H
