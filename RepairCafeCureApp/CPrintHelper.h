/*
Copyright(C) 2023/24  artvabas

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>

To see the license for this source code, please visit :
< https ://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit :
< https ://artvabas.com>
	< https ://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at :
< https ://artvabas.com/contact>

	*/

	/*
	* This file is part of RepairCafeCureApp.
	* File: CPrintHelper.cpp, defines the class CPrintHelper.
	*
	* This class is used to print the general header, footer and logo on a portrait printout.
	* The body of the printout is to be implemented in the class which defines this.
	*
	* Target: Windows 10/11 64bit
	* Version: 1.0.3.5 (beta)
	* Created: 16-09-2023, (dd-mm-yyyy)
	* Updated: 21-09-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* License: GPLv3
	*/
#pragma once
#include <memory>
namespace artvabas::rcc::support
{
	class CPrintHelper
	{
	private:
		CDC* m_pDC;

	public:
		CFont* m_pFont;
		CFont m_fontBoldHeader;
		CFont m_fontPlainBody;
		CFont m_fontBoldBody;
		std::unique_ptr<long>(m_pxPos);
		std::unique_ptr<long>(m_pyPos);
		const unsigned int m_pixEndPage;

	private:
		typedef unsigned int pixel;
		const pixel m_pixMargin;
		const pixel m_pixFontHeightHeader;
		const pixel m_pixFontHeightBody;
		const pixel m_pixPageXPos;
		CImage m_imgLogo;

	public:
		CPrintHelper(CDC* pDC);
		virtual ~CPrintHelper();

	public:
		inline pixel TotalTabInPixels(const pixel nTabs) const { return m_pixMargin * nTabs; }
		inline pixel HeaderTextLineDown(const pixel nLine) const { return m_pixFontHeightHeader * nLine; }
		inline pixel BodyTextLineDown(const pixel nLine) const { return m_pixFontHeightBody * nLine; }

		void PrintLogo();
		void PrintHeader(const CString& strHeader);
		void PrintFooter(const int nPage, const CString& strFooter = _T(""));
	};
}