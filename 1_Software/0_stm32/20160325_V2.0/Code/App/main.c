/**
*************************************************************************************************
* @file    main.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.18
* @brief   主函数
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.18   Ge Yue       主函数 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup MAIN main: APP Group
* @{
*/

#include "Manage.h"

 /**
 * @brief  main函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
int main()
{
    SystemInit();
    
    SoftwareInit();
    
    while(1)
    {
        if (ReqHeartBeat)
        {
            Task();
            ReqHeartBeat = 0;
        }
    }
}

/**  @} */

