/*
 *      Nana C++ Library - Creator
 *      Author: besh81
 */
#include "config.h"
#include <nana/gui/wvl.hpp>
#include <nana/gui/place.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/treebox.hpp>
#include <nana/gui/widgets/toolbar.hpp>
#include <nana/gui/filebox.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include "pugixml/pugixml.hpp"
#include "guimanager.h"
#include "imagemanager.h"
#include "filemanager.h"
#include "codegenerator.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"
#include "style.h"


using namespace nana;


guimanager		g_gui_mgr;	// manage all the gui elements
imagemanager	g_img_mgr;
filemanager		g_file_mgr;	// manage absolute and relative path
std::string		prj_name;


class nana_creator : public form
{
public:
	nana_creator()
		: form(API::make_center(1200, 700), appear::decorate<appear::sizable, appear::minimize, appear::maximize>())
	{
		caption(CREATOR_NAME " " CREATOR_VERSION);

		_place.div("vertical <weight=30 toolbar><<weight=20% vertical <assets>|<objects>>|<weight=55% canvas>|<props>>");

		_init_ctrls();

		_place.collocate();
	};

	~nana_creator()
	{
		g_gui_mgr.clear();
	}


	bool load_xml(const std::string& filename)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(filename.c_str());

		if(!result)
		{
			std::cout << "XML [" << filename << "] parsed with errors\n";
			std::cout << "Error description: " << result.description() << "\n";
			std::cout << "Error offset: " << result.offset << " (error at [..." << result.offset << "]\n\n";
			return false;
		}

		// read root node
		pugi::xml_node root = doc.child(NODE_ROOT);
		if(root.empty())
		{
			std::cout << "Error missing root node: " << NODE_ROOT << "\n";
			return false;
		}

		// deserialize the XML structure
		return g_gui_mgr.deserialize(&root);
	}


	bool save_xml(const std::string& filename)
	{
		pugi::xml_document doc;

		// append root node
		pugi::xml_node root = doc.append_child(NODE_ROOT);
		root.append_attribute("version") = CREATOR_VERSION;

		g_gui_mgr.serialize(&root);

		//doc.print(std::cout); //TEMP debug
		return doc.save_file(filename.c_str());
	}


	bool generate_cpp()
	{
		std::string path;
		auto tpos = prj_name.find_last_of("\\/");
		if(tpos != prj_name.npos)
			path = prj_name.substr(0, tpos);

		codegenerator cpp;
		cpp.generate(handle(), g_gui_mgr.get_root()->child, path);
		cpp.print(std::cout);
		return true;
	}


private:
	void _init_ctrls()
	{
		// toolbar
		_toolbar.scale(21);
		nana::paint::image _img_new;
		_img_new.open("icons/new_prj_dark.png");
		_toolbar.append("New project", _img_new);
		nana::paint::image _img_load;
		_img_load.open("icons/load_prj_dark.png");
		_toolbar.append("Load project", _img_load);
		nana::paint::image _img_save;
		_img_save.open("icons/save_dark.png");
		_toolbar.append("Save project", _img_save);
		_toolbar.separate();
		nana::paint::image _img_out;
		_img_out.open("icons/output_on_dark.png");
		_toolbar.append("Generate C++ code", _img_out);
		_toolbar.separate();
		nana::paint::image _img_del;
		_img_del.open("icons/delete_dark.png");
		_toolbar.append("Delete current selection", _img_del);
		_toolbar.separate();
		nana::paint::image _img_up;
		_img_up.open("icons/up_dark.png");
		_toolbar.append("Move up current selection", _img_up);
		nana::paint::image _img_down;
		_img_down.open("icons/down_dark.png");
		_toolbar.append("Move down current selection", _img_down);

		_toolbar.events().selected([this](const nana::arg_toolbar & arg)
		{
			if(arg.button == 0)
			{
				//TODO: messaggio di conferma
				g_gui_mgr.clear();
			}
			else if(arg.button == 1)
			{
				filebox fb(*this, true);
				fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
				fb.add_filter("All Files (*.*)", "*.*");

				if(fb())
				{
					//TODO: messaggio di conferma
					g_gui_mgr.clear();

					std::cout << "Load file: " << fb.file() << std::endl;
					load_xml(fb.file());
					prj_name = fb.file();
				}
			}
			else if(arg.button == 2)
			{
				filebox fb(*this, false);
				fb.add_filter("Nana Creator Project (*." PROJECT_EXT ")", "*." PROJECT_EXT);
				fb.init_file(prj_name);

				if(fb())
				{
					std::cout << "Save file: " << fb.file() << std::endl;
					save_xml(fb.file());
				}
			}
			else if(arg.button == 4)
			{
				generate_cpp();
			}
			else if(arg.button == 6)
			{
				// delete current selection
				g_gui_mgr.deleteselected();
			}
			else if(arg.button == 8)
			{
				// move up current selection
				g_gui_mgr.moveupselected();
			}
			else if(arg.button == 9)
			{
				// move down current selection
				g_gui_mgr.movedownselected();
			}
		});
		_place.field("toolbar") << _toolbar;

		// assets
		_place.field("assets") << _assets;

		// objects
		_place.field("objects") << _objects;
		

		// properties
		_place.field("props") << _properties;


		// canvas
		_canvas.bgcolor(CREATOR_MAIN_BG);
		_place.field("canvas") << _canvas;


		g_gui_mgr.root_wd(*this);
		g_gui_mgr.init(&_properties, &_assets, &_objects, &_canvas);


		//TEMP - main panel
		//g_gui_mgr.addmainpanel();
	}


	place				_place{ *this };

	resizablecanvas		_canvas{ *this };

	assetspanel			_assets{ *this };
	objectspanel		_objects{ *this };
	propertiespanel		_properties{ *this };

	toolbar				_toolbar{ *this };
};


void main()
{
	// init ctrls images
	g_img_mgr.add(CTRL_LAYOUT, "icons/horizontal_layout_dark.png");
	g_img_mgr.add(CTRL_BUTTON, "icons/button_dark.png");
	g_img_mgr.add(CTRL_LABEL, "icons/label_dark.png");
	g_img_mgr.add(CTRL_TEXTBOX, "icons/textbox_dark.png");
	g_img_mgr.add(CTRL_LISTBOX, "icons/listbox_dark.png");
	g_img_mgr.add(CTRL_PANEL, "icons/panel_dark.png");
	g_img_mgr.add(CTRL_COMBOX, "icons/combox_dark.png");
	g_img_mgr.add(CTRL_SPINBOX, "icons/spinbox_dark.png");
	g_img_mgr.add(CTRL_CHECKBOX, "icons/checkbox_dark.png");
	g_img_mgr.add(CTRL_DATECHOOSER, "icons/datechooser_dark.png");
	g_img_mgr.add(CTRL_TOOLBAR, "icons/toolbar_dark.png");
	g_img_mgr.add(CTRL_FORM, "icons/form_dark.png");
	g_img_mgr.add(CTRL_CATEGORIZE, "icons/categorize_dark.png");
	g_img_mgr.add(CTRL_GROUP, "icons/group_dark.png");
	g_img_mgr.add(CTRL_MENUBAR, "icons/menubar_dark.png");
	g_img_mgr.add(CTRL_PICTURE, "icons/picture_dark.png");
	g_img_mgr.add(CTRL_PROGRESS, "icons/progress_dark.png");
	g_img_mgr.add(CTRL_SLIDER, "icons/slider_dark.png");
	g_img_mgr.add(CTRL_TABBAR, "icons/tabbar_dark.png");
	g_img_mgr.add(CTRL_TREEBOX, "icons/treebox_dark.png");


	nana_creator fm;
	//fm.zoom(true);
	fm.show();
	exec();
}
/*
#include <nana/gui/wvl.hpp>
#include <nana/gui/widgets/tabbar.hpp>
#include <nana/gui/widgets/group.hpp>
#include <nana/gui/widgets/panel.hpp>
#include <nana/gui/widgets/listbox.hpp>
#include <nana/gui/widgets/button.hpp>
#include <string>
#include <set>
#include <functional>
#include <random>

using namespace std;
using namespace nana;


int main()
{
	form fm{ API::make_center(1000, 800), appear::decorate<appear::taskbar, appear::sizable>() };
	fm.bgcolor(colors::white);
	group gp1(fm, "Group 1"), gp2(fm, "Group 2");
	fm.div("vert margin=10");
	tabbar<panel<true>*> tb(gp1);
	tb.push_back("Page 1");
	tb.push_back("Page 2");
	panel<true> page1(gp1, true), page2(gp1, true);
	page1.bgcolor(colors::light_green);
	page2.bgcolor(colors::light_blue);
	tb.tab_bgcolor(0, colors::light_green);
	tb.tab_bgcolor(1, colors::light_blue);

	place plc1(page1);
	plc1.div("vert <listbox margin=15> <button weight=45 margin=[0,15,15,15]>");
	listbox lb1(page1);
	button btn1(page1, "press to add a new page");
	plc1["listbox"] << lb1;
	plc1["button"] << btn1;
	plc1.collocate();

	place plc2(page2);
	plc2.div("vert <button weight=60 margin=15> <listbox margin=[0,15,15,15]>");
	listbox lb2(page2);
	button btn2(page2, "press to add a new page");
	plc2["listbox"] << lb2;
	plc2["button"] << btn2;
	plc2.collocate();

	tb.attach(0, page1);
	tb.attach(1, page2);

	const size_t static_page_count = tb.length();

	using panel_ptr = unique_ptr<panel<true>>;
	using place_ptr = unique_ptr<place>;
	using widget_ptr = unique_ptr<widget>;

	set<panel_ptr> newpages;
	set<place_ptr> newplaces;
	set<widget_ptr> newwidgets;

	function<void(void)> newpagefn = [&]
	{
		auto &newpage = *newpages.emplace(new panel<true>{ gp1, true }).first;
		auto &newplace = *newplaces.emplace(new place{ *newpage }).first;
		auto &newlistbox = *newwidgets.emplace(new listbox{ *newpage }).first;
		auto &newbtn1 = *newwidgets.emplace(new button{ *newpage, "Add new page" }).first;
		auto &newbtn2 = *newwidgets.emplace(new button{ *newpage, "Remove this page" }).first;
		auto &newbtn3 = *newwidgets.emplace(new button{ *newpage, "Remove all new pages" }).first;

		static size_t pagecount(1);

		newbtn1->events().click(newpagefn);

		newbtn2->events().click([&]
		{
			size_t pos(static_page_count);
			for(; pos < tb.length(); pos++)
				if(tb.at(pos) == newpage.get()) break;
			tb.erase(pos);
			API::refresh_window(tb);
			newpages.erase(newpage);
			newplaces.erase(newplace);
			newwidgets.erase(newlistbox);
			newwidgets.erase(newbtn1);
			newwidgets.erase(newbtn2);
			newwidgets.erase(newbtn3);
		});

		newbtn3->events().click([&]
		{
			for(auto &page : newpages)
			{
				size_t pos(static_page_count);
				for(; pos < tb.length(); pos++)
					if(tb.at(pos) == page.get()) break;
				tb.erase(pos);
			}
			API::refresh_window(tb);
			newpages.clear();
			newplaces.clear();
			newwidgets.clear();
			pagecount = 1;
		});

		newplace->div("vert <listbox margin=15> <buttons gap=15 weight=45 margin=[0,15,15,15]>");
		newplace->field("listbox") << *newlistbox;
		newplace->field("buttons") << *newbtn1 << *newbtn2 << *newbtn3;
		newplace->collocate();

		size_t newtab_pos = static_page_count + newpages.size() - 1;
		tb.insert(newtab_pos, "New page #" + to_string(pagecount++), newpage.get());
		tb.attach(newtab_pos, *newpage);
		gp1["tabpage"].fasten(*newpage);
		gp1.collocate();

		mt19937 rng;
		rng.seed(random_device()());
		uniform_int_distribution<mt19937::result_type> dist(190, 255);
		color rcolor(dist(rng), dist(rng), dist(rng));
		tb.tab_bgcolor(newtab_pos, rcolor);
		newpage->bgcolor(rcolor);
	};

	btn1.events().click(newpagefn);
	btn2.events().click(newpagefn);

	gp1.div("vert margin=15 <tabbar weight=20> <tabpage>");
	gp1["tabbar"] << tb;
	gp1["tabpage"].fasten(page1);
	gp1["tabpage"].fasten(page2);
	gp1.collocate();
	fm[""] << gp1 << gp2;
	fm.collocate();
	fm.show();
	exec();
}
*/
