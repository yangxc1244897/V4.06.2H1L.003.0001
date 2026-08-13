#ifndef H_TL_ATTRIBUTE_TYPE_H
#define H_TL_ATTRIBUTE_TYPE_H

/*GraphObject*/
#define GRAPH_OBJECT	0
enum
{
	Object_TextContent = GRAPH_OBJECT,   // 文字内容，char*类型字符串或stAutoData_t类型, 设置内容后，自动调用Update更新内容
	Object_ShowTextFrame_B,   // 是否显示文本外框， bool
	Object_TextFontName,      // 文字字体名称，char*类型字符串
	Object_TextLenStyle,      // 文本的长度类型， 固定间距或是固定长度，如果是固定长度，需要指定字符串长度, int类型， 0固定长度， 1固定间距
	Object_TextFixedLen,      // 文本的固定长度，与长度类型搭配使用， double类型
	Object_Is2DBarcode_B,      // 是否是2DBarcode， BOOL类型
	Object_IsDouble_B,         // 是否启用双变量
	Object_Double_TextName,    // 双变量文字名称，char*类型字符串
	Object_Double_TextContent,   // 双变量文字内容，char*类型字符串或stAutoData_t类型, 设置内容后，自动调用Update更新内容
	Object_Shape_Basepoint_I,    // Shape类型基准点，int类型，值参考emRowAlign_t枚举
	Object_Text_Object_Type_I,   // 文本对象类型，int类型，值参考emTextObjectType_t枚举
	Object_2DBarcode_Index_I,      // 采用第几组2DBarcode， int类型
};

/*LASER*/
#define DEV_LASER	100
enum
{
	Laser_ShowPenDatabaseWnd_NULL = DEV_LASER,     /*! 显示激光参数库管理界面。*/
	Laser_ShowPenDatabaseSelectWnd_P,              /*! 显示激光参数库选择界面。IMarkPen* */
	Laser_GetPenDatabaseParam_P,                   /*! 获取激光参数库中的激光参数。IMarkPen* */
	Laser_SetPenOptionWndFlag_I,          /*! 在笔参数对话框之前设置，设置状态。Int */
};

enum {
	PenOptionWndFlag_ADD = 0,          //添加激光参数
	PenOptionWndFlag_VISIT,            //查看激光参数
	PenOptionWndFlag_EDIT,              //编辑激光参数
	PenOptionWndFlag_EDIT_PARAM         //编辑激光参数,只能修改内容
};

#endif