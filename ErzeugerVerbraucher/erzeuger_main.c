#include <memory.h>
#include <signal.h>
#include <stdio.h>
#include "ring_buffer.h"


#define ENTRY_NUMBER 200

static char *text = "Jesus of Suburbia\n"
        "\t\tby Greenday\n\n"
        "I'm the son of rage and love\n"
        "The Jesus of Suburbia\n"
        "The bible of none of the above\n"
        "On a steady diet of\n"
        "\n"
        "Soda pop and Ritalin\n"
        "No one ever died for my sins in hell\n"
        "As far as I can tell\n"
        "is the one's I got away with\n"
        "\n"
        "And there's nothing wrong with me\n"
        "This is how I'm supposed to be\n"
        "In a land of make believe\n"
        "That don't believe in me\n"
        "\n"
        "Get my television fix\n"
        "Sitting on my crucifix\n"
        "A living room or my private womb\n"
        "While the moms and Brads are away\n"
        "\n"
        "To fall in love and fall in debt\n"
        "To alcohol and cigarettes\n"
        "And Mary Jane to keep me insane\n"
        "Doing someone else's cocaine\n"
        "\n"
        "And there's nothing wrong with me\n"
        "This is how I'm supposed to be\n"
        "In a land of make believe\n"
        "That don't believe in me\n"
        "\n"
        "(Drum Solo)\n"
        "At the center of the earth, in the parking lot\n"
        "Of the 7-11 where I was taught the motto was just a lie\n"
        "It says \"Home is where your heart is\" but what a shame\n"
        "\n"
        "'Cause everyone's heart doesn't beat the same, it's beating out of time\n"
        "City of the dead at the end of another lost highway\n"
        "Signs misleading to nowhere\n"
        "City of the damned lost children with dirty faces today\n"
        "\n"
        "No one really seems to care\n"
        "I read the graffiti in the bathroom stall\n"
        "Like the holy scriptures of the shopping mall, and so it seemed to confess\n"
        "It didn't say much, but it only confirmed\n"
        "\n"
        "That the center of the earth is the end of the world, and I could really care less\n"
        "City of the dead at the end of another lost highway\n"
        "Signs misleading to nowhere\n"
        "City of the damned lost children with dirty faces today\n"
        "\n"
        "No one really seems to care\n"
        "I don't care if you don't\n"
        "I don't care if you don't\n"
        "I don't care if you don't care\n"
        "I don't care if you don't\n"
        "I don't care if you don't\n"
        "\n"
        "I don't care if you don't care\n"
        "I don't care if you don't\n"
        "I don't care if you don't\n"
        "I don't care if you don't care\n"
        "I don't care if you don't\n"
        "I don't care if you don't\n"
        "\n"
        "I don't care if you don't care\n"
        "\n"
        "I don't care\n"
        "Everyone's so full of shit\n"
        "Born and raised by hypocrites\n"
        "Hearts recycled but never saved\n"
        "\n"
        "From the cradle to the grave\n"
        "We are the kids of war and peace\n"
        "From Anaheim to the middle east\n"
        "We are the stories and disciples of\n"
        "\n"
        "The Jesus of Suburbia\n"
        "Land of make believe\n"
        "And it don't believe in me\n"
        "Land of make believe\n"
        "\n"
        "And it don't believe and I don't care\n"
        "I don't care\n"
        "I don't care\n"
        "I don't care\n"
        "\n"
        "I don't care\n"
        "Dearly beloved, are you listening?\n"
        "I can't remember a word that you were saying\n"
        "Are we demented or am I disturbed?\n"
        "\n"
        "The space that's in between insane and insecure\n"
        "Oh, therapy, can you please fill the void?\n"
        "Am I retarded or am I just overjoyed?\n"
        "Nobody's perfect and I stand accused\n"
        "\n"
        "For lack of a better word and that's my best excuse\n"
        "To live and not to breathe\n"
        "Is to die in tragedy\n"
        "To run, to run away\n"
        "\n"
        "To find what you believe\n"
        "And I leave behind\n"
        "This hurricane of fucking lies\n"
        "I lost my faith to this\n"
        "This town that don't exist\n"
        "So I run, I run away\n"
        "To the lights of masochists\n"
        "And I leave behind\n"
        "This hurricane of fucking lies\n"
        "And I walked this line\n"
        "A million and one fucking times\n"
        "But not this time\n"
        "I don't feel any shame, I won't apologize\n"
        "When there ain't nowhere you can go\n"
        "Running away from pain when you've been victimized\n"
        "Tales from another broken...\n"
        "Home\n"
        "You're leaving\n"
        "You're leaving\n"
        "You're leaving\n"
        "You're leaving home\n\n";

static void sig_handler(int signum) {
    if (signum == SIGUSR1) {
        ring_buffer_dealloc();
        ring_buffer_server_dealloc();
        exit(EXIT_SUCCESS);
    }
}

int main() {

    struct buffer_entry entries[ENTRY_NUMBER];
    struct sigaction action;
    int i;
    pid_t pid;
    int text_pos = 0;
    int text_len;

    memset(&action, 0, sizeof(action));
    action.sa_handler = sig_handler;
    if (sigaction(SIGUSR1, &action, NULL) < 0) {
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }

    ring_buffer_init();
    ring_buffer_server_init();

    pid = getpid();
    if (pid < 0) {
        perror("could not get pid");
        exit(EXIT_FAILURE);
    }

    text_len = strlen(text);
    for (i = 0; i < ENTRY_NUMBER; ++i) {
        int j;
        entries[i].number = i;
        entries[i].producer_pid = pid;
        for (j = 0; j < 31; ++j) {
            entries[i].text[j] = text[text_pos];
            text_pos = (text_pos + 1) % text_len;
        }
        entries[i].text[31] = '\0';
    }
    entries[ENTRY_NUMBER - 1].number = -1;

    ring_buffer_write(entries, ENTRY_NUMBER);
    pause();
    exit(EXIT_FAILURE);
}