/**
* Copyright (C) 2014-2050 SongJia
* All rights reserved.
*
* @file       event.h
* @brief	  �Զ�����Ϣ
* @version    v1.0
* @author     SongJia
* @date       2015-04-23
*
* Describe     ������NSIS�ű������ò�����Ϣ���ƣ�����ֱ�ӵ��ó��򷽷����������������ִ�С�
*/


#ifndef ___EVENT__H___
#define ___EVENT__H___
//����tab��ͼ�л�
#define WM_SJTABSETSEL (WM_USER+400)
//////��һ��ͼ Welcome ��Ϣ/////
//��ѡ��״̬���֪ͨ
#define WM_SJONECHECKSTATUS (WM_USER+300)
//����������Э����ͼ��Ϣ
#define WM_SJONELINKAGREEMENT (WM_USER+302)
//�����Զ��尲װ��ͼ��Ϣ
#define WM_SJONECUSTOMINSTALL (WM_USER+303)
//���밲װ������Ϣ
#define WM_SJONEINSTALL (WM_USER+304)

//////�ڶ���ͼ agreement ��Ϣ/////
#define WM_SJTWOSURE (WM_USER+310)

//////������ͼ choose ��Ϣ/////
//��һ����Ϣ
#define WM_SJTHREEPREVIOUS (WM_USER+320)
//���밲װ������Ϣ
#define WM_SJTHREEINSTALL WM_SJONEINSTALL
//���¿��ô��̿ռ�
#define WM_SJTHREEUPDATEFREESPACE (WM_USER+321)
//����������̿ռ�
#define WM_SJTHREEUPDATEREQUIREDSPACE (WM_USER+322)

//ֹͣ��Ϣѭ����ʹ��װ�������ִ�С�
#define WM_SJMSGLOOPSTOP (WM_USER+323)

//////������ͼ installing ��Ϣ/////

//////������ͼ finish ��Ϣ/////


/////ж��///////
////��һ��ͼ unstallstart////
#define WM_SJUNSTALLSTARTNEXT (WM_USER+353)


#endif