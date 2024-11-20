import { defineConfig } from 'tsup'

export default defineConfig({
  entry: [
    "./src/slv-mkscript.ts"
  ],
  platform: "node",
  format: "esm",
  dts: false,
  sourcemap: false,
  clean: true,
  bundle: true,
})
