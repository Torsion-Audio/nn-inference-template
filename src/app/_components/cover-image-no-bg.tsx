import cn from "classnames";
import Link from "next/link";
import Image from "next/image";

type Props = {
  title: string;
  src: string;
  slug?: string;
};

const CoverImageNoBg = ({ title, src, slug }: Props) => {
    return (
        <div>
          <Image
            src={src}
            alt={`Cover Image for ${title}`}
            className="w-full"
            width={1300}
            height={630}
          />
        </div>
      );
};

export default CoverImageNoBg;
