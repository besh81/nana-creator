/*
 *		nana::folderbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */

#ifndef NANA_CREATOR_FOLDERBOX_H
#define NANA_CREATOR_FOLDERBOX_H



namespace nana
{
	 /*! \class folderbox
	 */
	class folderbox
	{
		struct implement;

	public:
		folderbox();
		folderbox(window owner);
		folderbox(const folderbox&);
		~folderbox();

		folderbox& operator=(const folderbox&);

		/// Change owner window
		void owner(window);

		/// Set a new title for the dialog
		/// @param	string	a text for title
		/// @return the old title.
		::std::string title(::std::string new_title);

		/**	@brief	Suggest initial path used to locate a directory when the folderbox starts.
		*	@param	string	initial_directory a path of initial directory
		*/
		folderbox& init_path(const ::std::string& initial_directory);

		::std::string path() const;


		/// Display the folderbox dialog
		bool show() const;

		/// a function object method alternative to show() to display the folderbox dialog, 
		bool operator()() const
		{
			return show();
		}

	private:
		implement * impl_;
	};
}//end namespace nana

#endif //NANA_CREATOR_FOLDERBOX_H
