/******************
Name: Aviv Sayer
ID: 326552304
Assignment: ex5
*******************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #defs
#define bool            int
#define true            1
#define false           0

// constants for the main menu
#define WATCH_PLAYLISTS 1
#define ADD_PLAYLIST    2
#define REMOVE_PLAYLIST 3
#define EXIT            4

// constants for the playlist menu
#define SHOW_PLAYLIST   1
#define ADD_SONG        2
#define DELETE_SONG     3
#define SORT            4
#define PLAY            5
#define W_EXIT          6

// constants for the sorting options
#define BY_YEAR         1
#define BY_STREAMS_ASC  2
#define BY_STREAMS_DESC 3
#define ALPHABETICALLY  4

// #typedefs
typedef struct Song
{
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

typedef struct Playlist
{
    char* name;
    Song** pSongs;
    int songsLength;
} Playlist;

// pre-declaring functions
void watchPlaylist(Playlist* pPlaylist);
void showPlaylist(Playlist* pPlaylist);
bool within(double min, double value, double max);
void playSong(Song* pSong);
void printSongs(Playlist* pPlaylist);
void addSong(Playlist* pPlaylist);
void scanString(char** pString);
bool compare(Song* pSong1, Song* pSong2, int method);
void removePlaylist(Playlist*** ppPlaylists, int index, int length);
void deleteSong(Playlist* pPlaylist);
void sortPlaylist(Playlist* pPlaylist);
void playPlaylist(Playlist* playlist);

int main()
{
    // create the playlists dynamic array - [Playlist*, ..., Playlist*], with length of {playlistsLength}
    Playlist** pPlaylists = NULL;
    int playlistsLength = 0;

    // the chosen option in the menu.
    int option;
    while (true)
    {
        // print the menu
        printf("Please Choose:\n"
            "\t1. Watch playlists\n"
            "\t2. Add playlist\n"
            "\t3. Remove playlist\n"
            "\t4. exit\n");
        // scan the chosen option
        scanf(" %d", &option);
        switch (option)
        {
        case WATCH_PLAYLISTS:
            {
                // if the user chose to watch all the playlists, go to playlist choosing menu
                while (true)
                {
                    printf("Choose a playlist:\n");
                    int i = 0;
                    // print the menu
                    for (; i < playlistsLength; ++i)
                        printf("\t%d. %s\n", i + 1, pPlaylists[i]->name);
                    // print the option of getting back to the main menu
                    printf("\t%d. Back to main menu\n", i + 1);
                    // scan the chosen option
                    scanf(" %d", &option);
                    // if the user chose to go back to the main menu, break the loop
                    if (--option == playlistsLength)
                        break;
                    // otherwise, watch the chosen playlist
                    watchPlaylist(pPlaylists[option]);
                }
                break;
            }
        case ADD_PLAYLIST:
            {
                // if the user chose to add a playlist, add a new playlist to the playlists array
                printf("Enter playlist's name:\n");
                char *name = NULL;
                // scan the name of the playlist
                scanf(" ");
                scanString(&name);
                // update the size of the playlists array
                playlistsLength++;
                // allocate memory for the new playlist's pointer
                pPlaylists = realloc(pPlaylists, playlistsLength * sizeof(Playlist));
                // if the allocation failed, exit the program
                if (pPlaylists == NULL)
                    exit(1);
                // allocate memory for the new playlist itself
                pPlaylists[playlistsLength - 1] = malloc(sizeof(Playlist));
                // if the allocation failed, exit the program
                if (pPlaylists[playlistsLength - 1] == NULL)
                    exit(1);
                // update the new playlist's fields
                pPlaylists[playlistsLength - 1]->name = name;
                pPlaylists[playlistsLength - 1]->pSongs = NULL;
                pPlaylists[playlistsLength - 1]->songsLength = 0;
                break;
            }
        case REMOVE_PLAYLIST:
            {
                // if the use chose to remove a playlist, remove the chosen playlist from the playlists array
                printf("Choose a playlist:\n");
                int i = 0;
                // print the menu containing all the playlists
                for (; i < playlistsLength; ++i)
                    printf("\t%d. %s\n", i + 1, pPlaylists[i]->name);
                // print the option of getting back to the main menu (and cancel the removal)
                printf("\t%d. Back to main menu\n", i + 1);
                // scan the chosen option
                scanf(" %d", &option);
                // if the user did not choose to go back to the main menu, remove the playlist
                if (--option != playlistsLength)
                {
                    removePlaylist(&pPlaylists, option, playlistsLength);
                    // update the size of the playlists array accordingly
                    playlistsLength--;
                }
                // else, do nothing (back to main menu)
                break;
            }
        case EXIT:
            {
                // if the user chose to exit the program, free all the memory and exit the program
                // for every playlist
                for (int i = 0; i < playlistsLength; ++i)
                {
                    // for every song in the playlist
                    for (int j = 0; j < pPlaylists[i]->songsLength; ++j)
                    {
                        // free the song's fields
                        free(pPlaylists[i]->pSongs[j]->title);
                        free(pPlaylists[i]->pSongs[j]->artist);
                        free(pPlaylists[i]->pSongs[j]->lyrics);
                        free(pPlaylists[i]->pSongs[j]);
                    }
                    // free the playlist's fields
                    free(pPlaylists[i]->name);
                    free(pPlaylists[i]->pSongs);
                    free(pPlaylists[i]);
                }
                // and free the playlists array itself
                free(pPlaylists);
                printf("Goodbye!\n");
                return 0;
            }
        }
    }
}

/**
 * opens a menu for the user to do varius actions on the playlist
 * @param pPlaylist the playlist to watch
 */
void watchPlaylist(Playlist* pPlaylist)
{
    // print the playlist's name
    printf("playlist %s:\n", pPlaylist->name);
    // the chosen option in the menu
    int option;
    while (true)
    {
        // print the menu
        printf("\t1. Show Playlist\n"
            "\t2. Add Song\n"
            "\t3. Delete Song\n"
            "\t4. Sort\n"
            "\t5. Play\n"
            "\t6. exit\n");
        // scan the chosen option
        scanf(" %d", &option);
        switch (option)
        {
        case SHOW_PLAYLIST:
            {
                // if the user chose to show the playlist, show all the songs in the playlist and their details
                showPlaylist(pPlaylist);
                break;
            }
        case ADD_SONG:
            {
                // if the user chose to add a song, add a new song to the playlist
                addSong(pPlaylist);
                break;
            }
        case DELETE_SONG:
            {
                // if the user chose to delete a song, delete a song from the playlist
                deleteSong(pPlaylist);
                break;
            }
        case SORT:
            {
                // if the user chose to sort the playlist, sort the playlist by the chosen method
                sortPlaylist(pPlaylist);
                break;
            }
        case PLAY:
            {
                // if the user chose to play the playlist, play all the songs in the playlist
                playPlaylist(pPlaylist);
                break;
            }
        case W_EXIT:
            {
                // if the user chose to exit the playlist menu, break the loop and return t the main menu
                return;
            }
        }
    }
}

/**
 * shows (prints) all the songs in the playlist and their details
 * @param pPlaylist the playlist to show
 */
void showPlaylist(Playlist* pPlaylist)
{
    // print all the songs in the playlist and their details
    printSongs(pPlaylist);
    // then, open a menu for the user to choose if he wants to listen to a specific song
    while (true)
    {
        printf("choose a song to play, or 0 to quit:\n");
        // the option the user chose
        int option;
        // scan the chosen option
        scanf(" %d", &option);
        // if the user chose a valid song, play the song
        if (within(1, option, pPlaylist->songsLength))
        {
            playSong(pPlaylist->pSongs[option - 1]);
        }
        // else, quit and return to the playlist menu
        else
        {
            return;
        }
    }
}

/**
 * prints all the songs in the playlist and their details
 * @param pPlaylist the playlist to print
 */
void printSongs(Playlist* pPlaylist)
{
    // for every song in the playlist
    for (int i = 0; i < pPlaylist->songsLength; ++i)
    {
        // print the song's details
        printf("%d. Title: %s\n"
               "   Artist: %s\n"
               "   Released: %d\n"
               "   Streams: %d\n",
               i + 1,
               pPlaylist->pSongs[i]->title,
               pPlaylist->pSongs[i]->artist,
               pPlaylist->pSongs[i]->year,
               pPlaylist->pSongs[i]->streams);
    }
}

/**
 * plays a song
 * @param pSong the song to play
 */
void playSong(Song* pSong)
{
    // print the song's title and lyrics
    printf("Now playing %s:\n", pSong->title);
    printf("$ %s $\n", pSong->lyrics);
    // update the song's streams count
    pSong->streams++;
}

/**
 * adds a song to the playlist
 * @param pPlaylist the playlist to add the song to
 */
void addSong(Playlist* pPlaylist)
{
    // update the size of the songs array
    pPlaylist->songsLength++;
    // allocate memory for the new song's pointer
    pPlaylist->pSongs = realloc(pPlaylist->pSongs, pPlaylist->songsLength * sizeof(Song));
    // if the allocation failed, exit the program
    if (pPlaylist->pSongs == NULL)
        exit(1);
    // allocate memory for the new song itself
    pPlaylist->pSongs[pPlaylist->songsLength - 1] = malloc(sizeof(Song));
    // if the allocation failed, exit the program
    if (pPlaylist->pSongs[pPlaylist->songsLength - 1] == NULL)
        exit(1);
    // init the new song's fields to NULL, to enable scanning them
    pPlaylist->pSongs[pPlaylist->songsLength - 1]->title = NULL;
    pPlaylist->pSongs[pPlaylist->songsLength - 1]->artist = NULL;
    pPlaylist->pSongs[pPlaylist->songsLength - 1]->lyrics = NULL;
    // scan the new song's details
    printf("Enter song's details\n");
    // title
    printf("Title:\n");
    scanf(" ");
    scanString(&pPlaylist->pSongs[pPlaylist->songsLength - 1]->title);
    // artist
    printf("Artist:\n");
    scanf(" ");
    scanString(&pPlaylist->pSongs[pPlaylist->songsLength - 1]->artist);
    // year
    printf("Year of release:\n");
    scanf(" %d", &pPlaylist->pSongs[pPlaylist->songsLength - 1]->year);
    // lyrics
    printf("Lyrics:\n");
    scanf(" ");
    scanString(&pPlaylist->pSongs[pPlaylist->songsLength - 1]->lyrics);
    // init the new song's streams count to 0
    pPlaylist->pSongs[pPlaylist->songsLength - 1]->streams = 0;
}

/**
 *
 * @param pPlaylist the playlist to delete a song from
 */
void deleteSong(Playlist* pPlaylist)
{
    // print all the songs in the playlist and their details
    printSongs(pPlaylist);
    // open a query for the user to choose which song to delete
    printf("choose a song to delete, or 0 to quit:\n");
    // the option the user chose
    int option;
    // scan the chosen option
    scanf(" %d", &option);
    // move the chosen song to the end of the playlist
    for (int i = option; i < pPlaylist->songsLength; ++i)
    {
        // swap the chosen song with the next song
        void* temp = pPlaylist->pSongs[i];
        pPlaylist->pSongs[i] = pPlaylist->pSongs[i - 1];
        pPlaylist->pSongs[i - 1] = temp;
    }
    // update the size of the songs array
    pPlaylist->songsLength--;
    // free the chosen song's fields
    free(pPlaylist->pSongs[pPlaylist->songsLength]->title);
    free(pPlaylist->pSongs[pPlaylist->songsLength]->artist);
    free(pPlaylist->pSongs[pPlaylist->songsLength]->lyrics);
    // and free the song itself
    free(pPlaylist->pSongs[pPlaylist->songsLength]);
    // reallocate the memory for the songs array
    pPlaylist->pSongs = realloc(pPlaylist->pSongs, pPlaylist->songsLength * sizeof(Song));
    // if the reallocation failed, exit the program
    if (pPlaylist->pSongs == NULL)
        exit(1);
    // print a confirmation message to the user
    printf("Song deleted successfully.\n");
}

/**
 * sorts the playlist by the user's chosen method
 * @param pPlaylist the playlist to sort
 */
void sortPlaylist(Playlist* pPlaylist)
{
    // print the sorting options (the menu)
    printf("choose:\n"
        "1. sort by year\n"
        "2. sort by streams - ascending order\n"
        "3. sort by streams - descending order\n"
        "4. sort alphabetically\n");
    // the option the user chose
    int option;
    // scan the chosen option
    scanf(" %d", &option);
    // if the option is invalid, set it to the default option (ALPHABETICALLY)
    if (!within(BY_YEAR, option, ALPHABETICALLY))
        option = ALPHABETICALLY;
    // good, old, VERY inefficient bubble sort :)
    for (int i = 1; i < pPlaylist->songsLength; ++i)
    {
        for (int j = 0; j < pPlaylist->songsLength; ++j)
        {
            if (compare(pPlaylist->pSongs[i], pPlaylist->pSongs[i - 1], option))
            {
                void* temp = pPlaylist->pSongs[i];
                pPlaylist->pSongs[i] = pPlaylist->pSongs[i - 1];
                pPlaylist->pSongs[i - 1] = temp;
            }
        }
    }
    // print a confirmation message to the user
    printf("sorted\n");
}

/**
 * compares two songs by the user's chosen method
 * @param pSong1 the first song to compare
 * @param pSong2 the second song to compare
 * @param method the method to compare by
 * @return true if the first song is "smaller" than the second song, false otherwise
 */
bool compare(Song* pSong1, Song* pSong2, int method)
{
    switch (method)
    {
    case BY_YEAR:
        {
            // by year, ascending order
            return pSong1->year <= pSong2->year;
        }
    case BY_STREAMS_ASC:
        {
            // by streams, ascending order
            return pSong1->streams <= pSong2->streams;
        }
    case BY_STREAMS_DESC:
        {
            // by streams, descending order
            return pSong1->streams >= pSong2->streams;
        }
    case ALPHABETICALLY:
        {
            // alphabetically (by ascii), ascending order
            return strcmp(pSong1->title, pSong2->title) <= 0;
        }
    }
    // never reached, but the compiler doesn't know that ;)
    return 0;
}

/**
 * plays all the songs in the playlist
 * @param playlist the playlist to play
 */
void playPlaylist(Playlist* playlist)
{
    // for every song in the playlist
    for (int i = 0; i < playlist->songsLength; ++i)
    {
        // play the song
        playSong(playlist->pSongs[i]);
    }
}

/**
 * removes a playlist from the playlists array
 * @param ppPlaylists the playlists array's pointer
 * @param index the index of the playlist to remove
 * @param length the length of the playlists array
 */
void removePlaylist(Playlist*** ppPlaylists, int index, int length)
{
    // move the chosen playlist to the end of the playlists array
    for (int i = ++index; i < length; ++i)
    {
        void* temp = (*ppPlaylists)[i];
        (*ppPlaylists)[i] = (*ppPlaylists)[i - 1];
       (*ppPlaylists)[i - 1] = temp;
    }
    // free the chosen playlist's fields
    // for every song in the playlist
    for (int i = 0; i < (*ppPlaylists)[length - 1]->songsLength; ++i)
    {
        // free the song's fields
        free((*ppPlaylists)[length - 1]->pSongs[i]->title);
        free((*ppPlaylists)[length - 1]->pSongs[i]->artist);
        free((*ppPlaylists)[length - 1]->pSongs[i]->lyrics);
        // free the song itself
        free((*ppPlaylists)[length - 1]->pSongs[i]);
    }
    // free the playlist's fields
    free((*ppPlaylists)[length - 1]->name);
    free((*ppPlaylists)[length - 1]->pSongs);
    // and free the playlist itself
    free((*ppPlaylists)[length - 1]);
    // reallocate the memory for the playlists array accordingly
    *ppPlaylists = realloc(*ppPlaylists, (length - 1) * sizeof(Song));
    // if the reallocation failed, exit the program
    if (*ppPlaylists == NULL && length != 1)
        exit(1);
    // print a confirmation message to the user
    printf("Playlist deleted.\n");
}

/**
 * scans a string from the user into the given pointer
 * @param pString the string to scan
 */
void scanString(char** pString)
{
    // for every character in the string
    for (int i = 0; ; ++i)
    {
        // allocate the memory for the character
        *pString = realloc(*pString, (i + 1) * sizeof(char));
        // if the allocation failed, exit the program
        if (*pString == NULL)
            exit(1);
        // scan the character
        scanf("%c", &(*pString)[i]);
        // if the character is a newline or a carriage return, end the string
        if ((*pString)[i] == '\r' || (*pString)[i] == '\n')
        {
            (*pString)[i] = '\0';
            break;
        }
    }
}

/**
 * checks if a value is within a range
 * @param min the minimum value
 * @param value the value to check
 * @param max the maximum value
 * @return if the value is within the range
 */
bool within(double min, double value, double max)
{
    return min <= value && value <= max;
}
