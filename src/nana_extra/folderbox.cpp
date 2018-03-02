/*
 *		nana::folderbox Implementation
 *
 *      Nana C++ Library - Creator
 *      Author: besh81
 */


#include <nana/gui.hpp>
#include <nana/filesystem/filesystem_ext.hpp>

#if defined(NANA_WINDOWS)
#include <windows.h>
#include <shobjidl.h>
#include <nana/deploy.hpp>
#elif defined(NANA_POSIX)
#endif

#include "folderbox.h"


namespace fs = std::experimental::filesystem;
namespace fs_ext = nana::filesystem_ext;


namespace nana
{
	//class folderbox
	struct folderbox::implement
	{
		window owner;

		std::string title;
		std::string path;
	};


	folderbox::folderbox()
		: folderbox(nullptr)
	{
	}

	folderbox::folderbox(window owner)
		: impl_(new implement)
	{
		impl_->owner = owner;
#if defined(NANA_WINDOWS)
		auto len = ::GetCurrentDirectory(0, nullptr);
		if(len)
		{
			std::wstring path;
			path.resize(len + 1);
			::GetCurrentDirectory(len, &(path[0]));
			path.resize(len);

			impl_->path = to_utf8(path);
		}
#endif
	}

	folderbox::folderbox(const folderbox& other)
		: impl_(new implement(*other.impl_))
	{}

	folderbox::~folderbox()
	{
		delete impl_;
	}

	folderbox& folderbox::operator=(const folderbox& other)
	{
		if(this != &other)
			*impl_ = *other.impl_;
		return *this;
	}

	void folderbox::owner(window wd)
	{
		impl_->owner = wd;
	}

	std::string folderbox::title(std::string s)
	{
		impl_->title.swap(s);
		return s;
	}

	folderbox& folderbox::init_path(const std::string& ipstr)
	{
		if(ipstr.empty())
		{
			impl_->path.clear();
		}
		else
		{
			if(fs::is_directory(ipstr))
				impl_->path = ipstr;
		}
		return *this;
	}

	std::string folderbox::path() const
	{
		return impl_->path;
	}

	bool folderbox::show() const
	{
#if defined(NANA_WINDOWS)
		bool ret_val = false;

		nana::form fm(impl_->owner);

		IFileDialog *fd(nullptr);
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&fd));
		if(SUCCEEDED(hr))
		{
			fd->SetOptions(FOS_PICKFOLDERS);

			// set title
			auto wtitle = to_wstring(impl_->title);
			fd->SetTitle(wtitle.empty() ? nullptr : wtitle.c_str());

			// set initial path
			if(!impl_->path.empty())
			{
				auto wpath = nana::to_wstring(impl_->path);
				IShellItem* siFolder;
				hr = SHCreateItemFromParsingName(&wpath[0], NULL, IID_PPV_ARGS(&siFolder));

				if(SUCCEEDED(hr))
				{
					fd->SetFolder(siFolder);
					siFolder->Release();
				}
			}

			// show dialog
			fd->Show((HWND)fm.native_handle());
			IShellItem *si;
			hr = fd->GetResult(&si); // fails if user cancelled
			if(SUCCEEDED(hr))
			{
				PWSTR pwstr(nullptr);
				hr = si->GetDisplayName(SIGDN_FILESYSPATH, &pwstr);
				if(SUCCEEDED(hr))
				{
					ret_val = true;

					std::wstring ws(pwstr);
					impl_->path = nana::to_utf8(ws);

					CoTaskMemFree(pwstr);
				}
				si->Release();
			}
			fd->Release();
		}
#elif defined(NANA_POSIX)
		
#endif

		return ret_val;
	}//end class folderbox

}//end namespace nana
