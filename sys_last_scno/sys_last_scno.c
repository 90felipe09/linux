#include <linux/module.h> 
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h> // Necessário para inicialização do módulo (__init e __exit names)
#include <linux/fs.h> // File System required
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "last_scno"

//log_number é inútil
// static int log_number;
static int foo;

static struct kobject *sys_last_scno_kobject;

//tudo inutil
// static int dev_open(struct inode*, struct file*);
// static int dev_release(struct inode*, struct file*);
// static ssize_t dev_read(struct file*, char*, size_t, loff_t*);
// static ssize_t dev_write(struct file*, const char*, size_t, loff_t*);

// // OPEN
// static ssize_t dev_open(
//     struct inode *inodep, struct file *filep
// ){
//     printk(KERN_INFO "Abriu\n");
//     return 0;
// }

// // WRITE
// static ssize_t dev_write(
//     struct file *filep, const char *buffer, size_t len, loff_t *offset
// ){
//     printk(KERN_INFO "Isso é readonly\n");
//     return -EFAULT;
// }

// // RELEASE
// static int dev_release(struct inode *inodep, struct file *filep){
//     printk(KERN_INFO "Fechou\n");
//     return 0;
// }

// // READ
// static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
//     int errors = 0;
//     char *message = "O último scno é 403";
//     int message_len = strlen(message);

//     errors = copy_to_user(buffer, message, message_len);

//     return errors == 0 ? message_len : -EFAULT;
// }

// log_to_file_show
static ssize_t log_to_file_show(
    struct kobject *kobj, 
    struct kobj_attribute *attr, 
    char *buf){
        return sprintf(buf, "A ultima scno é %d\n", 403);
    }

// log_to_file_store. Por algum motivo esse cara não está sendo aceito
//  pelo __ATTR. Por isso estou usando NULL no lugar dele.
static ssize_t log_to_file_store(
    struct kobject *kobj,
    struct kobj_attribute *attr,
    char *buf,
    size_t count
){
    sscanf(buf, "%du", &foo);
    return count;
}

// Útil para o teletipo. Mas não vai nos ajudar agora
// static struct file_operations fops = {
//     .open = dev_open,
//     .read = dev_read,
//     .write = dev_write,
//     .release = dev_release,
// };

// Especifica os parâmetros do arquivo: um número de referência para o arquivo,
//  o número rwx (permissão), o que fazer quando ele for aberto, o que fazer quando
//  se armazenar um valor nele.
static struct kobj_attribute log_attribute = __ATTR(
    log_number,
    0660,
    log_to_file_show,
    NULL);

// Deleta a referência ao kobject.
static void __exit sys_last_scno_exit(void){
    // unregister chrdev foi para a solução por char device. não vai ser útil
    // unregister_chrdev(log_number, DEVICE_NAME);
    printk(KERN_INFO "Removido o módulo");
    // Deleta a referência ao kobject_put
    kobject_put(sys_last_scno_kobject);
}

// Chamado ao compilar.
static int __init sys_last_scno_init(void){
    int error;

    // Usado na tentativa de se usar um char device (teletipo) não resolve o nosso problema
    // log_number = register_chrdev(0, DEVICE_NAME, &fops);

    // Crio uma pasta chamada last_scno/ dentro de sys.
    sys_last_scno_kobject = kobject_create_and_add(DEVICE_NAME, kernel_kobj);
    // Tratativa de exceção
    if(!sys_last_scno_kobject){
        return -ENOMEM;
    }

    error = 0;
    // Crio um arquivo lá dentro com as propriedade que eu defini em log_attribute
    error = sysfs_create_file(sys_last_scno_kobject, &log_attribute.attr);

    // Tratativa de exceção
    // if (log_number < 0){
    //     pr_debug("Falhou ao criar o last_scno_log em sys/kernel/last_scno_log \n");
    //     printk(KERN_ALERT "Falhou ao criar o last_scno_log em sys/kernel/last_scno_log \n");
    // }

    // Printa lá na compilação alguma informação.
    // log_number só foi relevante para aquele negócio do teletipo. Não vai nos ajudar.
    pr_debug("\n\nMódulo sys_last_scno inicializado com sucesso!\n\n");
    // printk(KERN_INFO "\n\nMódulo sys_last_scno inicializado com sucesso! %d\n\n", log_number);
    printk(KERN_INFO "\n\nMódulo sys_last_scno inicializado com sucesso! \n\n");
    
    return 0;
}

// Indica que devo chamar essa função ao inicializar o módulo (compilá-lo)
module_init(sys_last_scno_init);
// Indica que devo chamar essa função ao sair
module_exit(sys_last_scno_exit);

// Cabeçalho de autoria
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felipe Kenzo Shiraishi <felhipeshiraishi@gmail.com>");
MODULE_DESCRIPTION("Logs the last system call number in table");