/*
CStatic��������࣬��������ɫ���ı��ؼ���
����SetTextColor�����ı���ɫ��
����Ҫ��ʱ�����SetWindowTextEx���ÿؼ��ı�
*/
#pragma once
#include "afxwin.h"
class CStaticEx :
	public CStatic
{
public:
	CStaticEx();
	~CStaticEx();

	//���뷽ʽ
	enum Alignment
	{
		LEFT,       //�����
		RIGHT,      //�Ҷ���
		CENTER,     //����
	};

	//��Static�ؼ�������������ɫ���ı�ʱ
public:
	void SetWindowTextEx(LPCTSTR lpszString, Alignment align = Alignment::LEFT);	//Ϊ�ؼ���������ɫ���ı�����Ҫ���SetTextColorʹ�ã�
	void SetTextColor(COLORREF textColor);		//���ÿؼ��ı���ɫ
	void SetBackColor(COLORREF back_color);		//���ÿؼ�������ɫ
	CString GetString() const;			//��ȡ�ؼ��ı�

protected:
	bool m_color_text{ false };
	COLORREF m_text_color;	//�ؼ�������ɫ
	COLORREF m_back_color;
	CString m_text;			//�ؼ��ϵ��ı�
	Alignment m_align{};		//�ı��Ķ��뷽ʽ
	bool m_draw_background_color{ false };	//�Ƿ���ҪΪ�ؼ���䱳����ɫ

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
};

