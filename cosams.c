#include "cosams.h"
#include <libdragon.h>

void stage_init(Stage* stage, const char* name) {
    strcpy(stage->name, name);
    stage->actor = NULL;
    memset(stage->actor_table, 0, STAGE_ACTORTABLE_MEMSIZE);
}
void stage_add_actor(Stage* stage, Actor* actor, bool indexed) {
    if (stage->actor) {
        linked_add_to_list(stage->actor->p_actor, stage->actor, actor, offsetof(Actor, p_actor), offsetof(Actor, n_actor));
    } else {
        stage->actor = actor;
    }
    if (indexed) {
        if (hash_add_pointer((void**)stage->actor_table, STAGE_ACTORTABLE_SIZE, actor->name, actor)) {
            actor->indexed = true;
        }
    }
}
void stage_life(Stage* stage, float delta) {
    Actor* current_actor = stage->actor;
    do {
        if (current_actor->enabled) {
            actor_life(current_actor, delta);
            current_actor = current_actor->n_actor;
        }
    }
    while(current_actor != stage->actor);
}
void stage_kill(Stage* stage) {
    if (stage->actor) {
        linked_kill_list(stage->actor, actor_simple_kill, offsetof(Actor, p_actor), offsetof(Actor, n_actor));
    }

    free(stage);
    stage = NULL;
}

void actor_init(Actor* actor, const char* name) {
    strcpy(actor->name, name);
    actor->module = NULL;
    actor->stage = NULL;
    actor->enabled = true;
    actor->indexed = false;
    actor->p_actor = actor;
    actor->n_actor = actor;
    memset(actor->module_table, 0, ACTOR_MODULETABLE_MEMSIZE);
    memset(actor->tags, 0, ACTOR_TAGTABLE_MEMSIZE);
}
void actor_add_module(Actor* actor, Module* module, bool indexed) {
    if (actor->module) {
        linked_add_to_list(actor->module->p_module, actor->module, module, offsetof(Module, p_module), offsetof(Module, n_module));
    } else {
        actor->module = module;
    }
    if (indexed) {
        if(hash_add_pointer((void**)actor->module_table, ACTOR_MODULETABLE_SIZE, module->name, module)) {
            module->indexed = true;
        }

        char debug_log_message[40];
        strcpy(debug_log_message, "Index failure on Actor '");
        strcpy(debug_log_message, actor->name);
        strcpy(debug_log_message, "'.");
        debug_log_write(debug_log_message);
    }
}
bool actor_add_tag(Actor* actor, const char* tag) {
    Tag* new_tag = malloc(sizeof(Tag));
    if(hash_add_pointer((void**)actor->tags, ACTOR_TAGTABLE_SIZE, tag, new_tag)) {
        strcpy(new_tag->tag, tag);
        return true;
    }

    char debug_log_message[40];
    strcpy(debug_log_message, "Tag failure on Actor '");
    strcpy(debug_log_message, actor->name);
    strcpy(debug_log_message, "'.");
    debug_log_write(debug_log_message);
    free(new_tag);
    return false;
}
bool actor_has_tag(Actor* actor, const char* tag) {
    return hash_get_pointer((void**)actor->tags, ACTOR_TAGTABLE_SIZE, tag, offsetof(Tag, tag)) >= 0;
}
void actor_pop_tag(Actor* actor, const char* tag) {
    Tag* pop_tag = hash_pop_pointer((void**)actor->tags, ACTOR_TAGTABLE_SIZE, tag, offsetof(Tag, tag));

    if (pop_tag)
        free(pop_tag);
}
void actor_life(Actor* actor, float delta) {
    Module* current_module = actor->module;
    do {
        if (current_module->enabled) {
            current_module->life(current_module, delta);
            current_module = current_module->n_module;
        }
    }
    while(current_module != actor->module);
}
void actor_kill(Actor* actor) {
    if (actor->stage && actor->indexed) {
        hash_pop_pointer((void**)actor->stage->actor_table, STAGE_ACTORTABLE_SIZE, actor->name, offsetof(Actor, name));
    }
    
    linked_pop_from_list(actor, offsetof(Actor, p_actor), offsetof(Actor, n_actor));

    actor_simple_kill(actor);
}
void actor_simple_kill(void* actor_pointer) {
    Actor* actor = (Actor*)actor_pointer;
    if (actor->module) {
        linked_kill_list(actor->module, module_simple_kill, offsetof(Module, p_module), offsetof(Module, n_module));
    }

    free(actor);
}

Module* module_create(const char* name) {
    Module* new_module = malloc(sizeof(Module));
    strcpy(new_module->name, name);
    new_module->enabled = true;
    new_module->indexed = false;
    new_module->actor = NULL;
    new_module->birth = NULL;
    new_module->active = NULL;
    new_module->life = NULL;
    new_module->inactive = NULL;
    new_module->death = NULL;
    new_module->p_module = new_module;
    new_module->n_module = new_module;
    return new_module;
}

void module_init(Module* module) {
    if (!module->birth)
        module->birth = module_birth;
    if (!module->active)
        module->active = module_active;
    if (!module->life)
        module->life = module_life;
    if (!module->inactive)
        module->inactive = module_inactive;
    if (!module->death)
        module->death = module_death;
    module->birth(module);
}
void module_kill(Module* module) {
    if (module->actor && module->indexed) {
        hash_pop_pointer((void**)module->actor->module_table, ACTOR_MODULETABLE_SIZE, module->name, offsetof(Module, name));
    }
       
    linked_pop_from_list(module, offsetof(Module, p_module), offsetof(Module, n_module));
    module_simple_kill(module);
}
void module_simple_kill(void* module_pointer) {
    Module* module = (Module*)module_pointer;
    module->death(module);
    free(module);
}

void module_birth(Module* self) {

}
void module_active(Module* self) {
    self->enabled = true;
}
void module_life(Module* self, float delta) {

}
void module_inactive(Module* self) {
    self->enabled = false;
}
void module_death(Module* self) {

}

Module* dblog_module_create(const char* name) {
    Module* new_module = module_create(name);
    new_module->death = dblog_module_death;
    module_init(new_module);
    return new_module;
}
void dblog_module_death(Module* self) {
    char text[30];
    strcpy(text, self->name);
    strcat(text, " DEATH");
    debug_log_write(text);
}