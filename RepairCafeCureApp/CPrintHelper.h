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

	private:
		typedef unsigned int pixel;
		const pixel m_pixMargin;
		const pixel m_pixFontHeightHeader;
		const pixel m_pixFontHeightBody;
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
		void PrintFooter(const CString& strFooter = _T(""));
	};
}

