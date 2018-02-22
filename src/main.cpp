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
#include "codegenerator.h"
#include "propertiespanel.h"
#include "assetspanel.h"
#include "objectspanel.h"
#include "resizablecanvas.h"
#include "style.h"


using namespace nana;


guimanager		g_gui_mgr;	// manage all the gui elements
imagemanager	g_img_mgr;
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
				std::string ext("*."); ext.append(PROJECT_EXT);
				fb.add_filter("Nana Creator Project", ext);
				fb.add_filter("All Files", "*.*");

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
				std::string ext("*." PROJECT_EXT);
				fb.add_filter("Nana Creator Project", ext);
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


	nana_creator fm;
	//fm.zoom(true);
	fm.show();
	exec();
}
