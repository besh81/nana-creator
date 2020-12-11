/*
 *		updater Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#include <iostream>
#include "updater.h"
#include "config.h"


// from version 0.27.0 use the new toolbar interface
// toolbar:
//		- scale is now tools_height
//		- items are changed based on type attribute
bool _update_prev_0_27_0(pugi::xml_node* root)
{
    struct walker : pugi::xml_tree_walker
    {
        virtual bool for_each(pugi::xml_node& node)
        {
            // toolbar
            if(std::string(node.name()) == CTRL_TOOLBAR)
            {
                if(node.attribute("scale").as_uint() > 0)
                    node.append_attribute("tools_height") = node.attribute("scale").as_string();
            }
            else if(std::string(node.name()) == NODE_ITEM)
            {
                // NODE_ITEM must have parent node

                // toolabr items
                if(std::string(node.parent().name()) == CTRL_TOOLBAR)
                {
                    if(node.attribute("separator").as_bool())
                    {
                        node.attribute("type") = "separator";
                    }
                    else if(node.attribute("goright").as_bool())
                    {
                        node.attribute("type") = "go_right";
                    }
                    else if(node.attribute("toggle_type").as_bool())
                    {
                        node.attribute("type") = "toggle";
                    }
                }
                // combox items
                else if(std::string(node.parent().name()) == CTRL_COMBOX)
                {
                    node.attribute("type") = "option";
                }
                // menubar items
                else if(std::string(node.parent().name()) == CTRL_MENUBAR)
                {
                    if(std::string(node.attribute("owner").as_string()).empty())
                    {
                        node.attribute("type") = "item";
                    }
                    else if(node.attribute("separator").as_bool())
                    {
                        node.attribute("type") = "separator";
                    }
                }
            }

            return true; // continue traversal
        }
    } _walker;

    root->traverse(_walker);

	return true;
}


bool updater(pugi::xml_node* root)
{
	std::string ver = root->attribute("version").as_string();
	if(ver < "0.27.0")
		return _update_prev_0_27_0(root);

	return true;
}
