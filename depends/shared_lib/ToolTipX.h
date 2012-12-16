#ifndef TOOLTIPX_H
#define TOOLTIPX_H

#include <boost/shared_ptr.hpp>
#include <deque>
#include "RichHtml.h"


class ToolTipX {
public:
	enum enPositioning {
		positionAuto=0,
		/// Tooltip "przyklejony" do myszki
		positionFloat = 1,
		positionFloatSlow = 2 | positionFloat,
		/// Zablokowany na pozycji "otwarcia"
		positionFirst = 0x10000,
		positionRefresh = 0x20000 | positionFirst,
		/// Ustalona pozycja
		positionFixed = 0x40000,
	};

	class iTip {
	public:
		virtual ~iTip(){};
		virtual void setSize(HDC hdc, RECT &rc)=0;
		virtual void paint(HDC hdc, RECT rc)=0;
		/** Tip podpinany jest do okna */
		virtual void attach(HWND hwnd) {}
		/** Tip odpinany jest od okna */
		virtual void detach(HWND hwnd) {}
		virtual int parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned) {returned = false; return 0;}
		static const int margin = 2;
	};

	typedef boost::shared_ptr<iTip> TipPtr;

	class iTarget {
	public:
		virtual ~iTarget(){}
		virtual bool mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local)=0;
		virtual bool operator==(const iTarget & b) const;
		virtual const char * getClass() const=0;
		virtual TipPtr getTip(ToolTipX * ttx)=0;
		virtual void onHide(ToolTipX * ttx) {};
		virtual void onShow(ToolTipX * ttx) {};
		virtual enPositioning getPositioning() {return positionAuto;}
		virtual int getDelay(ToolTipX * ttx) {return ttx->enterWait;}
		virtual void attach(ToolTipX * ttx);
		virtual void detach(ToolTipX * ttx);

	};

	typedef boost::shared_ptr<iTarget> TargetPtr;

	class Target:public iTarget {
	public:
		Target() {}
		Target(TipPtr tip):_tip(tip) {}
		virtual void setTip(ToolTipX * ttx, TipPtr tip);
		TipPtr getTip(ToolTipX * ttx) {return _tip;}
		// zawsze w obrêbie, ¿eby automat nam go nie wywali³
		bool mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local) {return true;}
		const char * getClass() const {return "Target";}
	protected:
		TipPtr _tip;
	};
	class TargetFixed:public Target {
	public:
		TargetFixed(TipPtr tip):Target(tip) {}
		enPositioning getPositioning() {return positionFixed;}
	};


	class TargetWindow: public Target{
	public:
		TargetWindow(TipPtr tip, HWND hwnd): _hwnd(hwnd), Target(tip) {}
		bool mouseAtTarget(ToolTipX * ttx, HWND window, POINT screen, POINT local);
		bool operator==(const iTarget & b) const {return iTarget::operator==(b) && static_cast<const TargetWindow&>(b)._hwnd == _hwnd;}
		const char *getClass() const {return "TargetWindow";}
	protected:
		HWND _hwnd;
	};
	class TargetId: public Target {
	public:
		TargetId(TipPtr tip, int id):_id(id), Target(tip) {}
		bool mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local);
		bool operator==(const iTarget & b) const {return iTarget::operator==(b) && static_cast<const TargetId&>(b)._id == _id;}
		const char * getClass() const {return "TargetId";}
	protected:
		int _id;
	};
	class TargetRect: public Target {
	public:
		TargetRect(TipPtr tip, RECT rc):_rect(rc), Target(tip) {}
		bool mouseAtTarget(ToolTipX * ttx,HWND window, POINT screen, POINT local);
		bool operator==(const iTarget & b) const;
		const char *getClass() const {return "TargetRect";}
	protected:
		RECT _rect;
	};

	class iTipCallBack: public iTip {
	public:
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void attach(HWND hwnd);
		void detach(HWND hwnd);
		int parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned);
		bool checkCache();
		virtual void fillCache()=0;
		virtual void resetCache() {_cachedTip.reset();}
	protected:
		TipPtr _cachedTip;
	};



	class TipText: public iTip {
	public:
		TipText(const CStdString & txt, HFONT font=0, int width = 0);
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void setText(const CStdString & text) {_text = text;}
		void setWidth(int width) {this->_width = width;}
		CStdString getText() {return _text;}
	protected:
		CStdString _text;
		HFONT _font;
		int _width;
	};
	class TipTextRich: public iTip {
	public:
		TipTextRich(const CStdString & txt, HFONT font=0, cRichEditFormat::fSetStyleCB styleCB=0, int width = 0);
		~TipTextRich();
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void setText(const CStdString & text);
		void setWidth(int width) {this->_width = width;}
		void attach(HWND hwnd);
		/** Tip odpinany jest od okna */
		void detach(HWND hwnd);
		int parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned);
		CStdString getText();
		static TipTextRich * fromHWND(HWND window);
	protected:
		HFONT _font;
		CStdString _text;
		cRichEditFormat::fSetStyleCB _styleCB;
		RECT _rect;
		HWND _rich;
		int _width;
		bool _positioned;
	};

	class TipOnlyImage : public iTip {
	public:
		TipOnlyImage(HBITMAP image, bool shared=false);
		~TipOnlyImage();
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void setImage(HBITMAP bmp, bool shared=false);
		void resetImage();
		HBITMAP getImage() {return _image;}

	protected:
		HBITMAP _image;
		bool _imageShared;
		int _imageW, _imageH;
	};
	class TipOnlyTitle : public iTip {
	public:
		TipOnlyTitle(const CStdString & title, HICON icon=0, bool iconShared=false, HFONT font=0);
		TipOnlyTitle(const CStdString & title, HIMAGELIST il, int pos, HFONT font=0);
		~TipOnlyTitle();
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void setIcon(HICON icon, bool iconShared=false);
		void setIcon(HIMAGELIST il, int pos);
		void resetIcon();
		void setFont(HFONT font);
		HICON getIcon() {return _icon;}
		void setTitle(const CStdString & title) {this->_title = title;}
		CStdString getTitle() {return _title;}
	private:
		HICON _icon;
		bool _iconShared;
		int _iconW, _iconH;
		CStdString _title;
		HFONT _font;
	};

	class Tip : public iTip {
	public:
		enum enAlign {
			alignAuto = 0,
			alignLeft = 1,
			alignRight = 2,
			alignWrap = 4
		};
		class Element {
		public:
			Element(TipPtr tip, RECT rc, enAlign align = alignAuto):tip(tip),rc(rc),align(align) {}
			iTip * operator->() {return tip.get();}
			TipPtr tip;
			RECT rc;
			enAlign align;
		};
		static TipPtr titleAndText(HICON icon, const CStdString & title, const CStdString & text, bool textRich=false, bool iconShared=false);
		static TipPtr titleTextAndImage(HICON icon, const CStdString & title, const CStdString & text, bool textRich, HBITMAP image, bool iconShared=false,bool imageShared=false);
		static TipPtr textAndImage(const CStdString & text, bool textRich, HBITMAP image, bool imageShared=false);
		void add(TipPtr tip, enAlign align = alignAuto);
		void add(iTip * tip, enAlign align = alignAuto) {
			this->add(TipPtr(tip), align);
		}
		void setSize(HDC hdc, RECT &rc);
		void paint(HDC hdc, RECT rc);
		void attach(HWND hwnd);
		void detach(HWND hwnd);
		bool empty() {return this->_elements.empty();}
		int parentMessageProc(HWND hwnd, int message, WPARAM wParam, LPARAM lParam, bool & returned);

	private:
		typedef deque<Element> tElements;
		static const int wrapWidth = 400;
		tElements _elements;
	};

	enum enPlacement {
		pNone = 0,
		pRight = 1,
		pLeft = 2,
		pBottom = 4,
		pTop = 8,
		pRightBottom = 5,
		pLeftBottom = 6,
		pRightTop = 9,
		pLeftTop = 10,
	};

	ToolTipX(HWND parent, HINSTANCE hInst);
	ToolTipX();
	void create(HWND parent, HINSTANCE hInst);
	~ToolTipX();
	static ToolTipX * fromHWND(HWND hwnd);
	/** 
	@param window oknie z kontrolkami
	*/
	void mouseMovement(HWND window, POINT screen, int scrollX=-1, int scrollY=-1);
	void mouseMovement(HWND window, POINT screen, POINT local);
	void mouseMovement(HWND window, int scrollX=-1, int scrollY=-1);
	/** Myszka "rusza" siê w kontrolce
	*/
//	void mouseMovementInTarget(HWND window, int x, int y);
	void hide(bool deactivate = false); // ukrywa tip'a
	void hideAuto(); // ukrywa tip'a
	void show(bool wait = false); // pokazuje tip'a
	bool visible();
	void refresh();
	void refreshRegion();
	void addTarget(HWND hwnd, TargetPtr &);
	void removeTarget(HWND hwnd, const TargetPtr &);
	void clearTargets(HWND hwnd);
	void setTip(HWND hwnd, TargetPtr &, bool treatAsAuto);
	void setPos(POINT screen, bool offset, enPositioning position, enPlacement place = pNone);
	POINT getPlacementPos(POINT screen, int offset, enPlacement place);
//	void setFloating(bool floating) {this->_lockedPos=!floating;}
	/** Ustawia tip'a i zeruje aktualny target.
	@warning przekazany obiekt zostanie USUNIÊTY! */
	void setTip(iTip *, bool treatAsAuto);
	void reset();
	void attach();
	TargetPtr getCurrentTarget() {return _currentTarget;}
	bool isCurrentTarget(iTarget * target) {return (_currentTarget.get() && target) && *_currentTarget.get() == *target;}
	HWND getTipWindow();
	HWND getCurrentWindow() {return _currentWindow;}
	bool isAutomated() {return this->_automated;}
	bool isAttached() {return this->_attached;}

	int enterWait; // czas oczekiwania na wyœwietlenie
	int enterSiblingTimeout;
	int hideTimeout;

private:
	typedef std::multimap<HWND, TargetPtr> tTargets;


	tTargets::iterator findTarget(HWND hwnd, const iTarget & target); 
	tTargets::iterator findTarget(HWND hwnd, const TargetPtr & target) {return findTarget(hwnd, *target.get());}
	static void registerClass(HINSTANCE hInst);
	static int CALLBACK windowProc(HWND hwnd , int message , WPARAM wParam, LPARAM lParam);
	void adjuctClientRect(RECT & rc);
	void adjuctTipRect(RECT & rc);
	static VOID CALLBACK timerShowHideProc(ToolTipX * tip , DWORD low , DWORD high);

	TargetPtr _currentTarget;
	HWND _currentWindow;
	HWND _hwnd;
	tTargets _targets;
	bool _active;
	HANDLE _timer;
	DWORD _lastShown;
	DWORD _lastPlacementSwitch;
	DWORD _lastPositioned;
	//DWORD _lastReading;
	HRGN _region;
	enPositioning _defaultPosition;
	bool _automated;
	bool _attached;
	static const int roundness = 4;
	static const int margin = 2;
	static const int marginLeft = 10;
	static const int cursorOffset = 10;
	SIZE _size;
	enPlacement _placement;
	POINT _lastScreen;
	int _dontReset;
};

#endif