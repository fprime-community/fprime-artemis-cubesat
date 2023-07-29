#!/usr/bin/python3
import sys
import argparse
import xml.etree.ElementTree as etree
from pathlib import Path
from xml.dom import minidom
from typing import List, Optional

def filter_section(root: etree.Element, sect_name: str):
    sect = root.find(sect_name)
    if sect is None:
        print('[ERROR] {} has no <commands> section'.format(sect_name))
        sys.exit(-1)


def filter_dictionary(dict_file: Path) -> etree.Element:
    d = etree.parse(dict_file).getroot()

    filter_section(d, 'commands')
    filter_section(d, 'events')
    filter_section(d, 'channels')

    return d


def element_exists(sect: etree.Element, key: str, val: str) -> bool:
    for elm in sect:
        if key in elm.attrib and elm.attrib[key] == val:
            return True
    return False


def merge_section(
    base: etree.Element,
    filtered: etree.Element,
    sect: str,
    merge_key: str,
    allow_duplicates: bool = True,
):
    new_sect = filtered.find(sect)
    base_sect = base.find(sect)
    if new_sect is None:
        return
    if base_sect is None:
        base.append(new_sect)
        return

    for elm in list(new_sect):
        val = elm.attrib[merge_key]
        if val is None:
            continue

        if element_exists(base_sect, merge_key, val):
            if not allow_duplicates:
                print('[ERROR] Conflicting entries in base and combined dictionary for {merge_key}={val} in section "{sect}"'.format(**locals()))
                sys.exit(-1)
        else:
            base_sect.append(elm)


def merge_dicts(
    base_dict: etree.Element, filtered_dict: etree.Element
) -> etree.Element:
    merge_section(base_dict, filtered_dict, 'enums', 'type')
    merge_section(base_dict, filtered_dict, 'serializables', 'type')
    merge_section(base_dict, filtered_dict, 'arrays', 'name')
    merge_section(base_dict, filtered_dict, 'events', 'id', allow_duplicates=False)
    merge_section(base_dict, filtered_dict, 'channels', 'id', allow_duplicates=False)
    merge_section(base_dict, filtered_dict, 'commands', 'opcode', allow_duplicates=False)
    return base_dict


def remove_indentation(root: etree.Element):
    root.tail = ''
    root.text = ''
    if len(root) > 0:
        for child in root:
            remove_indentation(child)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description='Combine multiple Fprime deployment dictionaries together.'
    )
    parser.add_argument(
        'base_dictionary',
        help='Dictionary to use as base of combined dictionary.',
    )
    parser.add_argument(
        'dictionary',
        help='Secondary dictionary to combine into base.',
    )
    parser.add_argument(
        'result',
        help='File to save the resulting combined dictionary into.',
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    base_dict_file = Path(args.base_dictionary)
    if not base_dict_file.exists():
        print('[ERROR] Dictionary file {} does not exist'.format(base_dict_file))
        return -1

    dict_file = Path(args.dictionary)
    if not dict_file.exists():
        print('[ERROR] Dictionary file {} does not exist'.format(dict_file))
        return -1

    filtered_dict = filter_dictionary(dict_file)
    base_dict = etree.parse(base_dict_file).getroot()
    combined_dict = merge_dicts(base_dict, filtered_dict)

    remove_indentation(combined_dict)
    result = minidom.parseString(etree.tostring(combined_dict)).toprettyxml(indent='  ')
    with open(args.result, 'w') as f:
        f.write(result)
    
    return 0
    

if __name__ == '__main__':
    sys.exit(main())