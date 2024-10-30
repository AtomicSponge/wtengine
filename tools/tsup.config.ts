import { defineConfig } from 'tsup'

export default defineConfig({
  entry: [
    "./src/wte-config.ts",
    "./src/wte-mkscript.ts",
    "./src/wte-syscheck.ts"
  ],
  platform: "node",
  format: "esm",
  dts: false,
  sourcemap: false,
  clean: true,
  bundle: true,
})
