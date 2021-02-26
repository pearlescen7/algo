def subset(list, ans, index):
    print(ans)
    while index < len(list):
        ans.append(list[index])
        subset(list, ans, index+1)
        ans.pop()
        index+=1
    
def print_subset(list):
    ans = []
    subset(list, ans, 0)

num_list = input().rstrip().split(" ")

for i in range(0, len(num_list)):
	num_list[i] = int(num_list[i])
	
print_subset(num_list)