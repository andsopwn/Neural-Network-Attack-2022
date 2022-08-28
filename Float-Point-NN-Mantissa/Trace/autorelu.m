relu = read('relu.bin', 2, 20000);
mlp  = read('mlp.bin', 255, 20000);

figure("Name", "ReLU INPUT : -1(1), 1(2)")
plot(relu(:,1))
figure("Name", "MLP INPUT : 0-255(1-256)")
plot(mlp(:,1))