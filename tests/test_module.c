#include "assert.h"
#include "module.h"
#include "printf.h"
#include "strings.h"

void test_get_content(){
    // proximity
    module_content_t* proximity_content = get_module_content(SD_MODULE_PROXIMITY);

    assert(proximity_content->numComponents == 1);
    assert(proximity_content->coordinates[0].x == 0);
    assert(proximity_content->coordinates[0].y == 0);
    assert(strcmp(proximity_content->components[0],"Proximity Module") == 0);
}

void main(void){
    module_init();

    test_get_content();
    printf("Test Module Successful \n");
}
