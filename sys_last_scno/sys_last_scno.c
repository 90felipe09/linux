#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/syscalls.h>

static struct kobject *sys_last_scno_kobject;
static int log_number;

static ssize_t print_to_log(
    struct kobject *kobj, 
    struct kobj_attribute *attr, 
    char *buf
){
    // Escrevo no buffer, ou seja, o arquivo, o número
    //  de syscalls disponíveis. Esta constante está
    //  disponível em syscall.tbl

    return sprintf(buf, "%d\n", 403);
}

// Não será utilizado, mas basicamente, o usuário pode escrever um número para
//  o arquivo last_scno_log
static ssize_t scan_to_log(
    struct kobject *kobj,
    struct kobj_attribute *attr,
    char *buf,
    size_t count
){
    sscanf(buf, "%du", &log_number);
    return count;
}

// Especifica os parâmetros do arquivo: um número de referência para o arquivo,
//  o número rwx (permissão), o que fazer quando ele for aberto, o que fazer quando
//  se armazenar um valor nele.
static struct kobj_attribute log_attribute = __ATTR(log_number,0660,print_to_log,scan_to_log);

// Chamado ao compilar.
static int __init sys_last_scno_init(void){
    int error;

    pr_debug("Módulo inicializado com sucesso!\n");

    sys_last_scno_kobject = kobject_create_and_add("last_scno_log", kernel_kobj);
    if(!sys_last_scno_kobject){
        return -ENOMEM;
    }

    error = 0;
    error = sysfs_create_file(sys_last_scno_kobject, &log_attribute.attr);
    if (error){
        pr_debug("Falhou ao criar o last_scno_log em sys/kernel/last_scno_log \n");
    }

    return error;
}

// Deleta a referência ao kobject.
static void __exit sys_last_scno_exit(void){
    pr_debug("Nada?AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    kobject_put(sys_last_scno_kobject);
}

module_init(sys_last_scno_init);
module_exit(sys_last_scno_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Kenzo Shiraishi <felhipeshiraishi@gmail.com>");
MODULE_DESCRIPTION("Logs the last system call number in table");