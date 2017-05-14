
for f in adlittle afiro agg agg2 agg3 25fv47 80bau3b ; do
    echo ${f}
    ./build/robust_cpp -m instances/${f}.mps -r instances/${f}_robust.json -a pessimization
    ./build/robust_cpp -m instances/${f}.mps -r instances/${f}_robust.json -a oco_opt
done
