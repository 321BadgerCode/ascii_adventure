#badger
"""
fix dialogue path numbers when use increment opposed to path system
"""
import re

"""
1: 1
2: 11
3: 12
4: 111
5: 112
6: 121
7: 122
8: 1111
9: 1112
10: 1121
11: 1122
"""
"""
#for binary paths(1-2, 2 values)
def generate_pattern(n:int)->str:
	if n==1:
		return '1'
	elif n%2==0:
		return generate_pattern(n//2)+'1'
	else:
		return generate_pattern(n//2)+'2'
"""
def generate_pattern(n:int,paths:int=2)->str:
	if n==1:
		return '1'
	return generate_pattern(n//paths)+str((n%paths)+1)

def replace_numbers_with_paths(match)->str:
	return '<'+generate_pattern(int(match.group(1)))+'>'

def process_dialogue_file(input_file, output_file):
	with open(input_file, 'r') as file:
		text = file.read()

	pattern = r'<(.*?)>'
	result = re.sub(pattern, replace_numbers_with_paths, text)

	with open(output_file, 'w') as file:
		file.write(result)

if __name__ == "__main__":
	dialogue_input_file = "./dlc/robot_uprising.txt"
	dialogue_output_file = "processed_dialogue.txt"

	process_dialogue_file(dialogue_input_file, dialogue_output_file)