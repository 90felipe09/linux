#include <linux/module.h> 
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/pid.h>

asmlinkage long sys_pid_father(int pid)
{
	task_struct current_task* = pid_task(find_vpid(pid), PIDTYPE_PID);

    return current_task->parent->pid;
}


// Deleta a referência ao kobject. a última coisa a ser feita para remover o módulo (Faz mais sentido em remoção manual de módulo)
static void __exit sys_last_scno_exit(void){
    printk(KERN_INFO "Removido o módulo sys_pid_father");
}

// Chamado ao compilar.
static int __init sys_last_scno_init(void){
    printk(KERN_INFO "\n\nMódulo sys_pid_father inicializado com sucesso! \n\n");
    
    return 0;
}

// Indica que devo chamar essa função ao inicializar o módulo (compilá-lo)
module_init(sys_last_scno_init);
// Indica que devo chamar essa função ao sair
module_exit(sys_last_scno_exit);

// Cabeçalho de autoria
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Kenzo Shiraishi <felhipeshiraishi@gmail.com>");
MODULE_DESCRIPTION("Returns pid of the father process of the given pid");