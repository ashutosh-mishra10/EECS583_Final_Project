import math
import sys

filename = sys.argv[1]

with open(filename,"r") as file:
    lines = file.readlines()

def extract(line):
    s = line.split(":")
    return s[1].strip()

def extractDomain(line):
    loop_depth=[]
    indices = line.split("|")[1].replace("DOMAIN: {","").split("[")[1].split("]")[0].replace(" ","").split(",")
    accessed_indices = line.strip().split("|")[2].split("->")[1].replace("}","").split("[")[1].split("]")[0].replace(" ","").split(",")
    r = line.split("|")[1].replace("DOMAIN:","").split(":")[1].split("and")
    ld = int(line.strip().split("|")[4].split(":")[1].replace(" ",""))    
    ranges = []
    dimensions = line.strip().split("|")[3].split(":")[1].replace(" ","")[:-1].split(",")
    for s in r:
        s = s.replace("}","").replace(" ","").strip().split("<=")
        ranges.append([int(s[0]),int(s[2])])
    return  [indices,accessed_indices,ranges,dimensions,ld]

def extractDomainOP(line):
    loop_depth=[]
    indices = line.split("|")[1].replace("DOMAIN: {","").split("[")[1].split("]")[0].replace(" ","").split(",")
    r = line.split("|")[1].replace("DOMAIN:","").split(":")[1].split("and")
    ranges = []
    for s in r:
        s = s.replace("}","").replace(" ","").strip().split("<=")
        ranges.append([int(s[0]),int(s[2])])
    return  [indices,ranges]

def findStride(access_indices,loop_index,ranges):
    #print(access_indices)
    #print(loop_index)
    #print(ranges)
    #print(type(loop_index))
    #print(f"len{len(loop_index)}")
    #print(type(access_indices[0]))
    hit_flag = False
    for idx_num,idx in enumerate(access_indices):
        if(loop_index not in idx and not hit_flag):
            #print(f"{loop_index} not in {idx}")
            stride = 0
            domain = 0
        else:
            if(loop_index in idx):
                stride = 1
                domain = idx_num
                hit_flag = True
    count = 0
    for idx in access_indices:
        if(count >= domain):
            break
        count += 1
        range_val = ranges[domain][1] - ranges[domain][0] + 1
        stride *= range_val
    return stride


total_latency_compute = 0
total_latency_data_access = 0

for line_num,line in enumerate(lines):
    if("VECTOR_OP" in line or "VECTOR_ACC" in line):
        [idx,rngs] = extractDomainOP(line)
        n_count = 1
        for rng in rngs:
            n_count *= rng[1] - rng[0] + 1
        n_count = math.ceil(n_count/8192)
    
        if("fadd" in line):
            latency = 351
        if("fmul" in line):
            latency = 346
        if("@min" in line):
            latency = 128
        if("@max" in line):
            latency = 128
        if("load" in line):
            latency = 32
        if("store" in line):
            latency = 32
        
        print(f"#{line_num} latency: {latency} count: {n_count} latency_compute: {latency * n_count}")
        total_latency_compute += latency * n_count

    if("VECTOR_ACC" in line):
        [idx,acc_idx,rngs,dims,lp_depth]=extractDomain(line)
        #print(acc_idx)
        #print(idx)
        #print(rngs)
        #print(lp_depth)
        #print(idx[lp_depth-1])
        stride = findStride(list(reversed(acc_idx)),idx[lp_depth-1],list(reversed(rngs)))
        n_cache_line = math.ceil(stride * 8192 * 4 / 64)
        n_cache_line = min(n_cache_line,8192)
        n_cache_line = max(n_cache_line,1)

        latency = n_cache_line * n_count / 2

        print(f"#{line_num} stride:{stride} cache_line: {n_cache_line} count: {n_count} latency_data_access {latency}")
        total_latency_data_access += latency

print(f"Compute: {total_latency_compute}")
print(f"Data access: {total_latency_data_access}")
