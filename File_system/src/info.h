/*
 *��ʾϵͳ��Ϣ���������̿������̿��С��ÿ���̿��������п�����
 */
void info()
{
	printf("\n%15s\n","ģ��Linux�ļ�ϵͳ��Ϣ:");
	printf("%15s  %10d %-3s\n","�̿�Ĵ�С:",_block_group[0].super_block.s_log_block_size,"�ֽ�");
	printf("%15s  %10d %-3s\n","ÿ����̿���:",_block_group[0].super_block.s_blocks_per_group,"��");
	printf("%15s  %10d %-3s\n","�ܵ��̿���:",_block_group[0].super_block.s_blocks_count,"��");
	printf("%15s  %10d %-3s\n","���п������:",_block_group[0].super_block.s_free_blocks_count,"��");
	printf("%15s  %10d %-3s\n","������Ϊ:",_block_group[0].super_block.s_blocks_count*BLOCK_SIZE,"�ֽ�");
	printf("%15s  %10d %-3s\n","ʣ��ռ�Ϊ:",_block_group[0].super_block.s_free_blocks_count*BLOCK_SIZE,"�ֽ�");
}