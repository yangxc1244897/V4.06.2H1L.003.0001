#pragma once


/**
* @brief 用户类型，新增用户不要修改顺序
*/
#define MAX_USER_TYPE_COUNT 4
typedef enum
{
	USER_A_NULL = -1,
	USER_A_ADMIN = 0,    // 管理员
	USER_A_ENGINEER,     // 工程师
	USER_A_TECH,         // 技术员
	USER_A_OPER,         // 操作员
}emUserAuthority_t;
/**
* @brief 用户操作类型
*/
typedef enum
{
	USER_O_ADJUST_POS = 0,    // 位置调整
	USER_O_LOAD_MOB_SET,      // 载入阵列配置
	USER_O_EDIT_PARAM,        // 修改激光参数
	USER_O_RESET_CONTENT,     // 重置内容
	USER_O_START_WORK,        // 开始作业
	USER_O_STOP_WORK,         // 停止作业
	USER_O_START_MARK,        // 开始标记
	USER_O_STOP_MARK,         // 停止标记
	USER_O_STRIPID_REPEAT,       // 检查strip id重复
	USER_O_WORK_MODE_SET,         // 作业模式
	USER_O_MES_PATH_SET,          // MES地址
	USER_O_PLC_COMM_SET,          // PLC通讯	
	USER_O_BASIC_SET,             // 基本设置
	USER_O_FILE_PATH_SET,         // 文件路径设置
	USER_O_POWER_MEASURE,         // 功率测量
	USER_O_LASER_SET,              // 激光器设置
	USER_O_IO_SET,                // IO设置
	USER_O_HARDWARE_SET,         // 硬件设置
	USER_O_MUTIL_CALIB_SET,      // 双头校正设置
	USER_O_CCD_NINE_SET,         // CCD九点标定设置
	USER_O_USER_MNG_SET,         // 用户管理设置
	USER_O_DEVICE_USE_SET,       // 设备管理设置
	USER_O_DEVICE_CLEAR_ZERO,              // 激光器时间清零
	USER_O_EDIT_TEXT_CONTENT,              // 修改文本内容
	USER_O_EDIT_PRINT_COUNT,              // 修改打印条数
	USER_O_IGNORE_CHECK_EQUIPMENTID,              // 检查过站信息
	USER_O_CLOSE_SW,              // 关闭软件
	USER_O_CHECK_MAPPING_INFO,              // 检查MAPPING信息
	USER_O_ENABLE_DUMMY,              // dummy
	USER_O_ENABLE_MAPPING,              // mapping
	USER_O_ENABLE_2D,              // 2D
	USER_O_ENABLE_VISION,              // 启用视觉定位


}emUserOperatorType_t;

const CString g_sUserOperatorCaption[] =
{
	_T("位置调整"),
	_T("载入阵列位置配置"),
	_T("修改激光参数"),
	_T("重置内容"),
	_T("开始作业"),
	_T("停止作业"),
	_T("开始标记"),
	_T("停止标记"),
	_T("StripID重复检查"),
	_T("作业模式"),
	_T("MES地址"),
	_T("PLC通讯"),
	_T("基本设置"),
	_T("文件路径设置"),
	_T("功率测量"),
	_T("激光器设置"),
	_T("IO设置"),
	_T("硬件设置"),
	_T("双头校正设置"),
	_T("CCD九点标定设置"),
	_T("用户管理设置"),
	_T("设备管理设置"),
	_T("设备使用时间清零"),
	_T("修改文本内容"),
	_T("修改打印条数"),
	_T("忽略过站管控"),
	_T("关闭软件"),
	_T("检查2D Barcode信息"),
	_T("首检选打"),
	_T("启用Mapping功能"),
	_T("启用2D读码器"),
	_T("启用视觉定位"),
};
const CString g_sUserOperatorDetails[] =
{
	_T("对阵列位置进行修改，不改变阵列文件，另存一份MOB修正文件"),
	_T("载入阵列位置修正MOB文件"),
	_T("修改激光参数，不能更改激光参数名称"),
	_T("重置内容，清空整个加载图档"),
	_T("软件进入自动模式，可以接收开始标记信号"),
	_T("软件退出自动模式，不能再接收开始标记信号"),
	_T("手动触发标记"),
	_T("停止标记"),
	_T("当扫入相同工单的条码时，警告提示"),
	_T("作业模式"),
	_T("MES地址"),
	_T("PLC通讯"),
	_T("系统设置-基本设置，还包括“启用dummy”"),
	_T("文件路径设置"),
	_T("功率测量"),
	_T("激光器设置"),
	_T("IO设置"),
	_T("硬件设置"),
	_T("双头校正设置"),
	_T("CCD九点标定设置"),
	_T("权限配置，和密码修改"),
	_T("设备管理设置"),
	_T("设备使用时间清零"),
	_T("修改文本内容"),
	_T("修改打印条数"),
	_T("不检查本机的过站管控信息，直接加载图档"),
	_T("关闭软件"),
	_T("Normal和Card模式下，2DBarcode打印判断"),
	_T("主页面-首检选打"),
	_T("系统设置-启用Mapping功能"),
	_T("系统设置-启用2D读码器"),
	_T("系统设置-启用视觉定位"),
};

// 管理员
const emUserOperatorType_t g_emDefaultAdminOperatorType[] = {
	USER_O_ADJUST_POS,       \
	USER_O_LOAD_MOB_SET,     \
	USER_O_EDIT_PARAM,       \
	USER_O_RESET_CONTENT,    \
	USER_O_START_WORK,       \
	USER_O_STOP_WORK,        \
	USER_O_START_MARK,       \
	USER_O_STOP_MARK, \
	USER_O_STRIPID_REPEAT,        \
	USER_O_WORK_MODE_SET,    \
	USER_O_MES_PATH_SET,     \
	USER_O_PLC_COMM_SET,     \
	USER_O_BASIC_SET,        \
	USER_O_FILE_PATH_SET,    \
	USER_O_POWER_MEASURE,    \
	USER_O_LASER_SET, \
	USER_O_IO_SET,           \
	USER_O_HARDWARE_SET,     \
	USER_O_MUTIL_CALIB_SET,  \
	USER_O_CCD_NINE_SET,     \
	USER_O_USER_MNG_SET,     \
	USER_O_DEVICE_USE_SET,   \
	USER_O_DEVICE_CLEAR_ZERO, USER_O_EDIT_TEXT_CONTENT, USER_O_EDIT_PRINT_COUNT, USER_O_IGNORE_CHECK_EQUIPMENTID, USER_O_CLOSE_SW, USER_O_CHECK_MAPPING_INFO, USER_O_ENABLE_DUMMY, USER_O_ENABLE_MAPPING, USER_O_ENABLE_2D, USER_O_ENABLE_VISION, 


};

// 工程师
const emUserOperatorType_t g_emDefaultEngineerOperatorType[] = {
	USER_O_ADJUST_POS,       \
	USER_O_LOAD_MOB_SET,     \
	USER_O_EDIT_PARAM,       \
	USER_O_RESET_CONTENT,    \
	USER_O_START_WORK,       \
	USER_O_STOP_WORK,        \
	USER_O_START_MARK,       \
	USER_O_STOP_MARK,        \
	USER_O_STRIPID_REPEAT, \
	USER_O_WORK_MODE_SET,    \
	USER_O_MES_PATH_SET,     \
	USER_O_PLC_COMM_SET,     \
	USER_O_BASIC_SET,        \
	USER_O_FILE_PATH_SET,    \
	USER_O_POWER_MEASURE,    \
	USER_O_DEVICE_USE_SET,   \
	USER_O_USER_MNG_SET, \
	USER_O_EDIT_TEXT_CONTENT, \
	USER_O_EDIT_PRINT_COUNT, \
	USER_O_IGNORE_CHECK_EQUIPMENTID, USER_O_CLOSE_SW, USER_O_CHECK_MAPPING_INFO, USER_O_ENABLE_DUMMY, USER_O_ENABLE_MAPPING, USER_O_ENABLE_2D, USER_O_ENABLE_VISION,
};

// 技术员
const emUserOperatorType_t g_emDefaultTechOperatorType[] = { 
	USER_O_ADJUST_POS,       \
	USER_O_RESET_CONTENT,    \
	USER_O_START_WORK,       \
	USER_O_STOP_WORK,        \
	USER_O_STRIPID_REPEAT,   \
	USER_O_BASIC_SET,        \
	USER_O_DEVICE_USE_SET,   \
	USER_O_EDIT_PRINT_COUNT, \
	USER_O_CLOSE_SW, USER_O_CHECK_MAPPING_INFO, USER_O_ENABLE_DUMMY
};

// 操作员
const emUserOperatorType_t g_emDefaultGuestOperatorType[] = {
	USER_O_START_WORK,       \
	USER_O_STOP_WORK,        \
};
