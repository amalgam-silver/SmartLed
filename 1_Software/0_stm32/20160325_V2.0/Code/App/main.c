/**
*************************************************************************************************
* @file    main.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.18
* @brief   ������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.18   Ge Yue       ������ 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup MAIN main: APP Group
* @{
*/

#include "Manage.h"

 /**
 * @brief  main����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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

