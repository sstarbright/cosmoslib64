#include "cosams.h"
#include <libdragon.h>

void stage_init(Stage* stage, const char* name) {
    strcpy(stage->name, name);
    stage->actor = NULL;
    memset(stage->actor_table, 0, STAGE_ACTORTABLE_MEMSIZE);
}
void stage_add_actor(Stage* stage, Actor* actor, bool indexed) {
    if (stage->actor) {
        linked_add_to_list(stage->actor->prev, stage->actor, actor, offsetof(Actor, prev), offsetof(Actor, next));
    } else {
        stage->actor = actor;
    }
    if (indexed) {
        if (hash_add_pointer((void**)stage->actor_table, STAGE_ACTORTABLE_SIZE, actor->name, actor)) {
            actor->indexed = true;
        }
    }
}
void stage_life(Stage* stage, float delta, uint32_t frame_buffer) {
    Actor* current_actor = stage->actor;
    do {
        if (current_actor->enabled) {
            actor_life(current_actor, delta, frame_buffer);
            current_actor = current_actor->next;
        }
    }
    while(current_actor != stage->actor);
}
void stage_kill(Stage* stage) {
    if (stage->actor) {
        linked_kill_list(stage->actor, actor_simple_kill, offsetof(Actor, prev), offsetof(Actor, next));
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
    actor->prev = actor;
    actor->next = actor;
    memset(actor->module_table, 0, ACTOR_MODULETABLE_MEMSIZE);
    memset(actor->tags, 0, ACTOR_TAGTABLE_MEMSIZE);
}
void actor_add_module(Actor* actor, Module* module, bool indexed) {
    if (module) {
        if (actor->module) {
            linked_add_to_list(actor->module->prev, actor->module, module, offsetof(Module, prev), offsetof(Module, next));
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
}
Module* actor_get_indexed_module(Actor* actor, const char* name) {
    int found_module = hash_get_pointer((void**)actor->module_table, ACTOR_MODULETABLE_MEMSIZE, name, offsetof(Module, name));
    if (found_module >= 0)
        return actor->module_table[found_module];
    return NULL;
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
void actor_life(Actor* actor, float delta, uint32_t frame_buffer) {
    Module* current_module = actor->module;
    do {
        if (current_module->enabled) {
            current_module->life(current_module, delta, frame_buffer);
            current_module = current_module->next;
        }
    }
    while(current_module != actor->module);
}
void actor_kill(Actor* actor) {
    if (actor->stage && actor->indexed) {
        hash_pop_pointer((void**)actor->stage->actor_table, STAGE_ACTORTABLE_SIZE, actor->name, offsetof(Actor, name));
    }
    
    linked_pop_from_list(actor, offsetof(Actor, prev), offsetof(Actor, next));

    actor_simple_kill(actor);
}
void actor_simple_kill(void* actor_pointer) {
    Actor* actor = (Actor*)actor_pointer;
    if (actor->module) {
        linked_kill_list(actor->module, module_simple_kill, offsetof(Module, prev), offsetof(Module, next));
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
    new_module->prev = new_module;
    new_module->next = new_module;
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
       
    linked_pop_from_list(module, offsetof(Module, prev), offsetof(Module, next));
    module_simple_kill(module);
}
void module_simple_kill(void* module_pointer) {
    Module* module = (Module*)module_pointer;
    module->death(module);
    free(module);
}

void module_birth(Module* _) {

}
void module_active(Module* self) {
    self->enabled = true;
}
void module_life(Module* self, float delta, uint32_t frame_buffer) {

}
void module_inactive(Module* self) {
    self->enabled = false;
}
void module_death(Module* _) {

}

Module* renderable_module_create(const char* name) {
    Module* new_module = module_create(name);
    new_module->birth = renderable_module_birth;
    new_module->death = renderable_module_death;
    
    RenderableModule* ren_module = malloc(sizeof(RenderableModule));
    new_module->data = ren_module;
    ren_module->module = new_module;
    ren_module->draw = NULL;
    return new_module;
}
void renderable_module_birth(Module* self) {
    if (!((RenderableModule*)self->data)->draw)
        ((RenderableModule*)self->data)->draw = renderable_module_draw;
}
void renderable_module_draw(RenderableModule* _, float __, uint32_t ___) {
    
}
void renderable_module_death(Module* module) {
    free((RenderableModule*)module->data);
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