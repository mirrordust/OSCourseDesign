/*************************************************************************//**
 *****************************************************************************
 * @file   misc.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   2008
 *****************************************************************************
 *****************************************************************************/

/* Orange'S FS */

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"
#include "hd.h"
#include "fs.h"

/*****************************************************************************
 *                                search_file
 *****************************************************************************/
/**
 * Search the file and return the inode_nr.
 *
 * @param[in] path The full path of the file to search.
 * @return         Ptr to the i-node of the file if successful, otherwise zero.
 * 
 * @see open()
 * @see do_open()
 *****************************************************************************/
PUBLIC int search_file(char * path)
{
	int i, j;

	char filename[MAX_PATH];
	memset(filename, 0, MAX_FILENAME_LEN);
	struct inode * dir_inode;
	//printl("%s\n", "88888888888888888888");
	if (strip_path(filename, path, &dir_inode) != 0) {
		//printl("%s\n", "THIS IS88888888888888888888");
		return 0;
	}
	//printl("%s\n", "88888888888888888888");
//dir_inode = root_inode;
	if (filename[0] == 0)	/* path: "/" */
		return dir_inode->i_num;

	/**
	 * Search the dir for the file.
	 */
	int dir_blk0_nr = dir_inode->i_start_sect;
	int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE - 1) / SECTOR_SIZE;
	int nr_dir_entries =
	  dir_inode->i_size / DIR_ENTRY_SIZE; /**
					       * including unused slots
					       * (the file has been deleted
					       * but the slot is still there)
					       */
	int m = 0;
	struct dir_entry * pde;
	for (i = 0; i < nr_dir_blks; i++) {
		RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
		pde = (struct dir_entry *)fsbuf;
		for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
			if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
			{
				//printf("pde->inode_nr:%d\n", pde->inode_nr);
				return pde->inode_nr;
			}
			if (++m > nr_dir_entries)
				break;
		}
		if (m > nr_dir_entries) /* all entries have been iterated */
			break;
	}

	/* file not found */
	//printf("%s\n", "is 7777");
	return 0;
}

/*****************************************************************************
 *                                strip_path
 *****************************************************************************/
/**
 * Get the basename from the fullpath.
 *
 * In Orange'S FS v1.0, all files are stored in the root directory.
 * There is no sub-folder thing.
 *
 * This routine should be called at the very beginning of file operations
 * such as open(), read() and write(). It accepts the full path and returns
 * two things: the basename and a ptr of the root dir's i-node.
 *
 * e.g. After stip_path(filename, "/blah", ppinode) finishes, we get:
 *      - filename: "blah"
 *      - *ppinode: root_inode
 *      - ret val:  0 (successful)
 *
 * Currently an acceptable pathname should begin with at most one `/'
 * preceding a filename.
 *
 * Filenames may contain any character except '/' and '\\0'.
 *
 * @param[out] filename The string for the result.
 * @param[in]  pathname The full pathname.
 * @param[out] ppinode  The ptr of the dir's inode will be stored here.
 * 
 * @return Zero if success, otherwise the pathname is not valid.
 *****************************************************************************/
/*PUBLIC int strip_path(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	const char * s = pathname;
	char * t = filename;

	if (s == 0)
		return -1;

	if (*s == '/')
		s++;

	while (*s) {		// check each character 
		if (*s == '/')
			return -1;
		*t++ = *s++;
		//if filename is too long, just truncate it 
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;

	*ppinode = root_inode;

	return 0;
}
*/
PUBLIC int strip_path(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	const char * s = pathname;
	char * t = filename;

	if (s == 0) {
		//printl("%s\n", "888888888888888888888");
		return -1;
	}

	if (*s == '/')
		s++;

	//printl("%s\n", "88888888888888888888");
	int n = 0;
	int mm = 0;
	while(*s){
		if (*s == '/')
		{
			n++;
		}
		mm++;
		s++;
		if (mm > 200)
			break;
	}
	//printf(">>%s\n", pathname);
	//printl(">>%d\n", n);
	s = pathname;
	
	int i, j;
	struct inode * dir_inode;

	if (n == 0)
	{
		if (*s == '/')
		s++;
		while (*s) {		// check each character 
			if (*s == '/')
				return -1;
			*t++ = *s++;
			//if filename is too long, just truncate it 
			if (t - filename >= MAX_FILENAME_LEN)
				break;
		}
		*t = 0;
		*ppinode = root_inode;
		return 0;
	}
	else if (n == 1)
	{
		//printf("%s\n", "22222<");
		if (*s == '/') 
		{
			s++;
		}
		while(*s != '/'){
			*t++ = *s++;
			if (t - filename >= MAX_FILENAME_LEN)
				break;
		}
		*t = 0;
		printf("filename:%s\n", filename);
		dir_inode = root_inode;
		int dir_blk0_nr = dir_inode->i_start_sect;
		int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE-1) / SECTOR_SIZE;
		int nr_dir_entries = 
		  dir_inode->i_size / DIR_ENTRY_SIZE;			 	
		int m = 0;
		int find_ = 0;
		struct dir_entry * pde;
		for (i = 0; i < nr_dir_blks; i++) {
			RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
			pde = (struct dir_entry *)fsbuf;
			for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
				//printf("@%d@%s@%s@\n",pde->inode_nr, pde->name,filename);
				if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
				{
					dir_inode = get_inode(dir_inode->i_dev, pde->inode_nr);
					//printf("%s\n", "==>>zhao dao le");
					*ppinode = dir_inode;
					t = filename;
					if (*s == '/') 
					{
						s++;
					}
					while(*s != '/'){
						*t++ = *s++;
						if (t - filename >= MAX_FILENAME_LEN)
							break;
					}
					*t = 0;
					printf("==>>%s\n", filename);
					printf("%s\n", "==>>zhao dao le");
					return 0;
					find_ = 1;
					/*if (dir_inode == 0)
					{
						printf("%s\n", "0000000");
					}
					else{
						printf("%s\n", "1111111");
						break;
					}*/
					//return pde->inode_nr;
				}
				if (++m > nr_dir_entries)
					break;
			}
			if (m > nr_dir_entries) /* all entries have been iterated */
				break;
			if (find_ == 1)
			{
				break;
			}
		}
		if (m > nr_dir_entries) {
			printl("%s\n", "zaizheli");
			//printf("%d\n", nr_dir_entries);
			return -1;
		}
		t = filename;
		if (*s == '/') 
		{
			s++;
		}
		while(*s){
			*t++ = *s++;
			if (t - filename >= MAX_FILENAME_LEN)
				break;
		}
		*t = 0;
		printf("second_filename:%s\n", filename);
		*ppinode = dir_inode;
		return 0;

	}
	else
	{
		int k;
		for (k = 0; k <= n; k++)
		{
			if (*s == '/')
			{
				s++;
			}
			while(*s != '/' && *s){
				*t++ = *s++;
				if (t - filename >= MAX_FILENAME_LEN)
					break;
			}
			*t = 0;
			//printf("%s\n", filename);
			if (k == 0)
			{
				dir_inode = root_inode;
				int dir_blk0_nr = dir_inode->i_start_sect;
				int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE-1) / SECTOR_SIZE;
				int nr_dir_entries = 
			  	dir_inode->i_size / DIR_ENTRY_SIZE;
			 	//printf("%d\n", nr_dir_entries);

				int m = 0;
				struct dir_entry * pde;
				for (i = 0; i < nr_dir_blks; i++) {
					RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
					pde = (struct dir_entry *)fsbuf;
					//printf("+++%s\n", filename);
					//printf("+++%s\n", pde->name);
					for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
						if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
						{
							dir_inode = get_inode(dir_inode->i_dev, pde->inode_nr);
							if (dir_inode == 0)
							{
								printf("%s\n", "0000000");
							}
							else{
								printf("%s\n", "1111111");
								break;
							}
							//return pde->inode_nr;
						}
						if (++m > nr_dir_entries)
							break;
					}
					if (m > nr_dir_entries) /* all entries have been iterated */
						break;
				}
				if (m > nr_dir_entries) {
					//printl("%d\n", m);
					//printf("%d\n", nr_dir_entries);
					return -1;
				}			
			}
			else if (k < n)
			{
				int dir_blk0_nr = dir_inode->i_start_sect;
				int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE-1) / SECTOR_SIZE;
				int nr_dir_entries = 
			  	  dir_inode->i_size / DIR_ENTRY_SIZE;

				int m = 0;
				struct dir_entry * pde;
				for (i = 0; i < nr_dir_blks; i++) {
					RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
					pde = (struct dir_entry *)fsbuf;
					for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
						if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
						{
							dir_inode = get_inode(dir_inode->i_dev, pde->inode_nr);
							//return pde->inode_nr;
							break;
						}
						if (++m > nr_dir_entries)
							break;
					}
					if (m > nr_dir_entries) /* all entries have been iterated */
						break;
				}
				if (m > nr_dir_entries) 
					return -1;
			}
			else
			{
				int dir_blk0_nr = dir_inode->i_start_sect;
				int nr_dir_blks = (dir_inode->i_size + SECTOR_SIZE-1) / SECTOR_SIZE;
				int nr_dir_entries = 
			  	  dir_inode->i_size / DIR_ENTRY_SIZE;

				int m = 0;
				struct dir_entry * pde;
				for (i = 0; i < nr_dir_blks; i++) {
					RD_SECT(dir_inode->i_dev, dir_blk0_nr + i);
					pde = (struct dir_entry *)fsbuf;
					for (j = 0; j < SECTOR_SIZE / DIR_ENTRY_SIZE; j++,pde++) {
						if (memcmp(filename, pde->name, MAX_FILENAME_LEN) == 0)
							break;
							//dir_inode = get_inode(dir_inode->i_dev, pde->inode_nr);
							//return pde->inode_nr;
						if (++m > nr_dir_entries)
							break;
					}
					if (m > nr_dir_entries) /* all entries have been iterated */
						break;
				}
				if (m > nr_dir_entries) 
					return -1;
			}
			t = filename;
		}
		*ppinode = dir_inode;

		return 0;
	}
	
}


PUBLIC int strip_path2(char * filename, const char * pathname,
		      struct inode** ppinode)
{
	const char * s = pathname;
	char * t = filename;

	if (s == 0)
		return -1;

	if (*s == '/')
		s++;

	while (*s) {		// check each character 
		if (*s == '/')
			return -1;
		*t++ = *s++;
		//if filename is too long, just truncate it 
		if (t - filename >= MAX_FILENAME_LEN)
			break;
	}
	*t = 0;

	*ppinode = root_inode;

	return 0;
}