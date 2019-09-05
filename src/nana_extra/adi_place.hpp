/**
 *	@file nana/gui/adi_place.hpp
 */

#ifndef NANA_GUI_ADI_PLACE_HPP
#define NANA_GUI_ADI_PLACE_HPP
#include <nana/push_ignore_diagnostic>
#include <nana/gui/basis.hpp>
#include <memory>
#include <functional>

namespace nana
{
	namespace paint
	{
		class graphics;
	}

	class widget;


	enum class dockposition
	{
		up,
		down,
		left,
		right
	};


	/// paneinfo class defined to specify the appearance of a dockpane
	class paneinfo
	{
	public:
		// Default constructor
		paneinfo() = default;
		paneinfo(const std::string& id)
		{
			data_.id = id;
		}

		bool empty() const
		{
			return data_.id.empty();
		}

		std::string id() const
		{
			return data_.id;
		}


		paneinfo& caption(const std::string& name);
		std::string caption() const;

		paneinfo& show_caption(bool show);
		bool show_caption() const;

		paneinfo& show_close(bool show);
		bool show_close() const;

		paneinfo& weightXXX(double value);
		double weightXXX() const;

		paneinfo& center(bool value);
		bool center() const;

		paneinfo& operator=(const paneinfo&) = default;


	private:
		struct data
		{
			std::string id;

			std::string caption;			///<  The caption of the pane
			bool show_caption{ true };		///<  Indicates where the pane caption should be visible
			bool close_button{ true };		///<  Indicates where the close button should be visible
			double weight{ 0.f };			///<  Indicates the weight of the pane in percentage along the place direction (0 <= not defined)
			bool center{ false };			///<  Indicates the pane is a center pane
		} data_;
	};



    ///  Layout management - an object of class adi_place is attached to a widget, and it automatically positions and resizes the children widgets.
	class adi_place
		: ::nana::noncopyable
	{
		struct implement;

	public:
		class error :public std::invalid_argument
		{
		public:
			error(	const std::string& what, const adi_place& plc);

			std::string base_what;
			std::string owner_caption;  ///< truncate caption (title) of the "adi_placed" widget
		};

		adi_place();
		adi_place(window);	///< Attaches to a specified widget.
		~adi_place();

		/** @brief Bind to a window
		 *	@param handle	A handle to a window which the adi_place wants to attach.
		 *	@remark	It will throw an exception if the adi_place has already binded to a window.
		 */
		void bind(window handle);
		window window_handle() const;

		void splitter_renderer(std::function<void(window, paint::graphics&, mouse_action)> fn);

		void collocate();	///< Layouts the widgets.

		/// Add a panel
		paneinfo add_pane(const std::string& id, widget* window, const std::string& caption = "");
		paneinfo add_pane(const std::string& id, widget* window, dockposition position, const std::string& caption = "");
		paneinfo add_pane(widget* window, const paneinfo& info);
		paneinfo add_pane(widget* window, dockposition position, const paneinfo& info);

		paneinfo get_pane(const std::string& id);

		void update_pane(const paneinfo& info);

	private:
		implement * impl_;
	};

	
}//end namespace nana
#include <nana/pop_ignore_diagnostic>

#endif //#ifndef NANA_GUI_ADI_PLACE_HPP
