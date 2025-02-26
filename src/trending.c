#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio_ext.h>
#include <ctype.h>
#include <time.h>
#include "trending.h"


void init_trending_topics(trending_topics *ttopics, unsigned capacity) {
    ttopics->topics = calloc(capacity, sizeof(topic));
    ttopics->size = 0;
    ttopics->capacity = capacity;
}

int add_new_topic(trending_topics *ttopics, char *new_topic_name) {
    topic new_topic;
    topic *new_topics_array;

    if (ttopics == NULL || strlen(new_topic_name) == 0) {
        return 1;
    }

    if (ttopics->size == ttopics->capacity) {
        new_topics_array = realloc(ttopics->topics, (ttopics->capacity + 1) * sizeof(topic));

        if (new_topics_array == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        } else {
            ttopics->topics = new_topics_array;
            ttopics->capacity++;
        }
    }

    strncpy(new_topic.name, new_topic_name, sizeof(new_topic.name));
    new_topic.posts_count = 1;

    if (ttopics->size == 0) {
        ttopics->topics[0] = new_topic;
        ttopics->size++;
        return 0;
    }
    for (unsigned i = 0; i < ttopics->size; i++) {
        if (strcmp(ttopics->topics[i].name, new_topic_name) == 0) {
            return 1;
        }

        if (ttopics->topics[i].posts_count == 1) {
            for (unsigned j = ttopics->size; j > i; j--) {
                ttopics->topics[j] = ttopics->topics[j - 1];
            }

            ttopics->topics[i] = new_topic;
            ttopics->size++;
            return 0;
        }
    }

    ttopics->topics[ttopics->size] = new_topic;
    ttopics->size++;

    return 0;
}

int increment_topic(trending_topics *ttopics, char *topic_name) {
    unsigned current_index;
    topic temp_topic;
    char has_error;

    has_error = 1;

    if (ttopics == NULL || strlen(topic_name) == 0 || ttopics->size == 0) {
        return 1;
    }

    for (unsigned i = 0; i < ttopics->size; i++) {
        if (strcmp(ttopics->topics[i].name, topic_name) == 0 && ttopics->topics[i].posts_count > 0) {
            ttopics->topics[i].posts_count++;
            current_index = i;
            has_error = 0;
            break;
        }
    }

    for (unsigned i = 0; i < ttopics->size; i++) {
        if (ttopics->topics[current_index].posts_count >= ttopics->topics[i].posts_count) {
            temp_topic = ttopics->topics[current_index]; 

            for (unsigned j = current_index; j > i; j--) {
                ttopics->topics[j] = ttopics->topics[j - 1];
            }

            ttopics->topics[i] = temp_topic;
            has_error = 0;
            break;
        } 
    }

    return has_error;
}

int is_hashtag_end(char c) {
    if (isalnum(c) == 0 && c != '_') {
        return 1;
    }

    return 0;
}

int get_trending_topics(trending_topics *ttopics) {
    FILE *posts_ptr;
    char current_post[500];
    char *username, *time, *text;
    char topic[50];
    char topic_found;
    char topic_exist;
    unsigned topic_start;

    posts_ptr = fopen(POSTS_FILE, "r");

    if (posts_ptr == NULL){
        printf("Error while reading %s\n", POSTS_FILE);
        return 1;
    }


    while (fgets(current_post, 500, posts_ptr) != NULL) {
        username = strtok(current_post, "|");
        time = strtok(NULL, "|");
        text = strtok(NULL, "\n");

        for (unsigned i = 0; text[i] != '\0'; i++) {
            if (text[i] == '#') {
                topic[0] = '\0';
                topic_found = 1;
                topic_start = i + 1;
            } else if (topic_found == 1 && is_hashtag_end(text[i]) == 0 && i - topic_start + 1 < sizeof(topic)) {
                topic[i - topic_start] = text[i];
                topic[i - topic_start + 1] = '\0';

                if (topic_found == 1 && is_hashtag_end(text[i + 1]) == 1 && topic[0] != '\0') {
                    topic_exist = 0;

                    for (unsigned j = 0; j < ttopics->size; j++) {
                        if (j == 0) {
                            topic_exist = 0;
                        }

                        if (strcmp(ttopics->topics[j].name, topic) == 0) {
                            topic_exist = 1;
                            break;
                        }
                    }

                    if (topic_exist) {
                        increment_topic(ttopics, topic);
                    } else {
                        add_new_topic(ttopics, topic);
                    }

                    topic_found = 0;
                }
            }
        }
    }

    return 0;
}

int show_trending_topics(unsigned max_topics) {
    trending_topics *ttopics;
    unsigned topics_limit;
    unsigned topic_min_length;
    unsigned padding;

    if (max_topics == 0) {
        max_topics = 5;
    }

    ttopics = malloc(sizeof(trending_topics));

    init_trending_topics(ttopics, max_topics);

    if (get_trending_topics(ttopics)) {
        return 1;
    }

    topics_limit = ttopics->size <= max_topics ? ttopics->size : max_topics;

    for (unsigned i = 0; i < topics_limit; i++) {
        if (i == 0) {
            topic_min_length = strlen(ttopics->topics[0].name);
        } else if (strlen(ttopics->topics[i].name) > topic_min_length) {
            topic_min_length = strlen(ttopics->topics[i].name);
        }
    }

    printf("Trending Topics:\n");

    for (unsigned i = 0; i < topics_limit; i++) {
        padding = topic_min_length - strlen(ttopics->topics[i].name);

        printf("#%s", ttopics->topics[i].name);

        for (unsigned j = 0; j < padding; j++) {
            printf(" ");
        }

        printf(" - %u\n", ttopics->topics[i].posts_count);
    }

    return 0;
}
