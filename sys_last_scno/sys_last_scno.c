#include <linux/module.h> 
#include <linux/printk.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#define DIR_NAME "last_scno"
static unsigned long last_number;

static struct kobject *sys_last_scno_kobject;

// log_to_file_show(), método que escreve para um buffer, no caso, o arquivo, uma mensagem.
static ssize_t log_to_file_show(
    struct kobject *kobj, 
    struct kobj_attribute *attr, 
    char *buf){
        return sprintf(buf, "A ultima scno é %s\n", "403");
    }


//  Especificação dos atributos deste arquivo.
//      O primeiro parâmetro confesso que ainda não compreendi de todo a natureza
//      O segundo parâmetro é um código de atribuição de rwx (Leitura Escrita e Execução).
//          Como é 660, 110, significa que é permitida a leitura e escrita.
//      O terceiro parâmetro é a função que deve ser engatilhada caso o arquivo seja lido, no caso, log_to_file_show.
//      O quarto é para o que fazer quando escrevemos no arquivo. Só queremos ler dele, então botei null.
static struct kobj_attribute log_attribute = __ATTR(
    last_number,
    0660,
    log_to_file_show,
    NULL);

// Deleta a referência ao kobject. a última coisa a ser feita para remover o módulo (Faz mais sentido em remoção manual de módulo)
static void __exit sys_last_scno_exit(void){
    printk(KERN_INFO "Removido o módulo");
    // Deleta a referência ao kobject_put
    kobject_put(sys_last_scno_kobject);
}

// Chamado ao compilar.
static int __init sys_last_scno_init(void){
    int error;

    // Crio uma pasta chamada last_scno/ dentro de sys.
    sys_last_scno_kobject = kobject_create_and_add(DIR_NAME, kernel_kobj);
    // Tratativa de exceção
    if(!sys_last_scno_kobject){
        return -ENOMEM;
    }

    error = 0;
    // Crio um arquivo lá dentro com as propriedade que eu defini em log_attribute
    //  Pelo que eu entendi, isso define uma propriedade para o diretório.
    //  Todo arquivo criado dentro desta pasta terá a propriedade definida do segundo argumento.
    error = sysfs_create_file(sys_last_scno_kobject, &log_attribute.attr);
    if(error){
        kobject_put(sys_last_scno_kobject);
        pr_debug("Não foi possível criar o arquivo");
        return error;
        
    }

    pr_debug("\n\nMódulo sys_last_scno inicializado com sucesso!\n\n");
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